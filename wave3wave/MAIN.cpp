// EXAMPLE OF COMPILING AND RUNNING PROGRAM
//  g++ -O3 -Wall -Wextra -pedantic -std=c++14  MAIN.cpp inputparser.cpp floatsignal.cpp -o MAIN -Wl,-rpath,opencv-3.1.0  -lopencv_core -lopencv_imgproc -lopencv_ml -lsndfile && valgrind --leak-check=full -v ./MAIN -a test_cv -i 4

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <tuple>
#include <algorithm>
// #include <cmath>
// #include <random>
// #include <chrono>
#include "sndfile.hh"
#include "inputparser.h"
#include "floatsignal.h"

#include <opencv2/opencv.hpp>
// own dependencies

// own header

// namespacing
using namespace std;
//using namespace chrono;


////////////////////////////////////////////////////////////////////////////////////////////////////
/// TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
// implement sampledown
// implement some different optimization heuristic (split file into parts?)
// export D-list (parser?), allow continuating session list



////////////////////////////////////////////////////////////////////////////////////////////////////
/// CLI
////////////////////////////////////////////////////////////////////////////////////////////////////

// -a action
// -s originalName, WHOLE PATH
// -m mat1 mat2 ... WHOLE PATHS
// -r sampledown ratio
// -i iterations
// -p project name
// -d the D-list





////////////////////////////////////////////////////////////////////////////////////////////////////
/// HELPING DS AND FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////
struct d_entry{string wavPath; int delay; double k;};

float* cv_crosscorrelate(float* orig, int origLen, float* mat, int matLen){
  // declare original
  IplImage* origCV = cvCreateImage(cvSize(origLen, 1), IPL_DEPTH_32F, 1); // last 1 is num of chans
  memcpy((float*)origCV->imageData, orig, origLen*sizeof(float));
  // declare material
  IplImage* matCV = cvCreateImage(cvSize(matLen, 1), IPL_DEPTH_32F, 1);
  memcpy((float*)matCV->imageData, mat, matLen*sizeof(float));
  // declare result container
  // create array for the result
  int ccLen = origLen-matLen+1;
  IplImage* ccCV = cvCreateImage(cvSize(ccLen,1), IPL_DEPTH_32F, 1);
  // perform CC and copy result to float array
  cvMatchTemplate(origCV, matCV, ccCV, CV_TM_CCORR);
  float* cc = new float[ccLen]();
  memcpy(cc, (float*)ccCV->imageData, ccLen*sizeof(float));
  // release allocated memory
  cvReleaseImage(&origCV);
  cvReleaseImage(&matCV);
  cvReleaseImage(&ccCV);
  return cc;
}

double energy(float* arr, int size){
  return inner_product(arr, arr+size, arr, 0.0);
}




////////////////////////////////////////////////////////////////////////////////////////////////////
/// MAIN ROUTINE
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv){
  InputParser input(argc, argv);

  if (input.getAction() == "test_cv"){
    if (!input.getIterations() || !input.getSampledownRatio()){
      cout << "aborting: action " << input.getAction() << " requires -i <N> iterations, "
           << "and -r <M> sampledown ratio" << endl;
      return -1;
    }
    vector<d_entry> D;
    const int DS_RATIO = input.getSampledownRatio();
    //
    string origPath = "child-short.wav";
    FloatSignal original(origPath);
    float* origContent = original.getContent();
    SF_INFO* origInfo = original.getSFInfo();
    int origSize = original.getSize();
    int origDownSize = original.getSize()/DS_RATIO;
    float* origDownContent = original.getDownsampledCopy(DS_RATIO);


    //
    for (unsigned int i=0; i<input.getIterations(); ++i){
      cout << endl << "LOOP " << i << endl;
      string matPath = "anvil.wav";
      cout << "pick and load " << matPath << endl;    
      FloatSignal material(matPath);
      float* matContent = material.getContent();
      float* matDownContent = material.getDownsampledCopy(DS_RATIO);
      SF_INFO* matInfo = material.getSFInfo();
      int matSize = material.getSize();
      int matDownSize = material.getSize()/DS_RATIO;
      //double matEnergy = energy(matContent, matSize);
      double matDownEnergy = energy(matDownContent, matDownSize);
      if (origInfo->samplerate!=matInfo->samplerate){
        cout << "ERROR: incompatible sample rates: " << origInfo->samplerate << ", " <<
          matInfo->samplerate << ". Aborting" << endl;
        return -1;
      }
      if (origInfo->channels!=matInfo->channels){
        cout << "ERROR: incompatible number of channels: " << origInfo->channels << ", " <<
          matInfo->channels << ". Aborting" << endl;
        return -1;
      }
      cout << "calculate cc and optimization parameters" << endl;
      float* cc = cv_crosscorrelate(origDownContent, origDownSize, matDownContent, matDownSize);
      auto minmax = minmax_element(cc, cc+origDownSize-matDownSize+1);
      int optPos_dw = ((abs(*minmax.first) > abs(*minmax.second))? minmax.first : minmax.second)-cc;
      double k_factor_approx = cc[optPos_dw] / matDownEnergy; // downsampling introduces considerable error


      //
      cout << "add opt to d-list and subtract from original" << endl;
      int optPos = optPos_dw*DS_RATIO;

      // float* cc2 = cv_crosscorrelate(origContent, origSize, matContent, matSize);
      // auto mm2 = minmax_element(cc2, cc2+origSize-matSize+1);
      // int max2 = ((abs(*mm2.first) > abs(*mm2.second))? mm2.first : mm2.second)-cc2;
      // double korig = cc2[max2]/matEnergy;

      
      // cout << ">>>>>>cc_orig:" << cc2[max2] << "  cc down: " << cc[maxPos_dw] << endl;
      // cout <<">>>>>>>>>k: " << korig << "    k down: " << k_factor_approx << endl;

      
      D.push_back(d_entry{matPath, optPos, k_factor_approx});
      for (int i=0; i<matSize; ++i){
        int delIdx = i+optPos;
        if(delIdx<0 || delIdx >= origSize){continue;}
        else{
          origContent[delIdx] -= matContent[i]*k_factor_approx;
          // if (i%DS_RATIO==0){
          //   origDownContent[(i/DS_RATIO)+optPos_dw] -= matDownContent[i/DS_RATIO]*k_factor_approx;
          // }
        }
      }

      for (int i=0; i<matDownSize; ++i){
        int delIdx = i+optPos_dw;
        if(delIdx<0 || delIdx >= origDownSize){continue;}
        else{origDownContent[delIdx] -= matDownContent[i]* k_factor_approx;}
      }
      
      delete[] cc;
      delete[] matDownContent;
    }

    cout << "reconstructing and saving to wav..." << endl;
    FloatSignal reconstruction(origPath);
    float* reconsContent= reconstruction.getContent();
    for (int i=0; i<origSize; ++i){
      reconsContent[i] -= origContent[i];
    }
    reconstruction.toWav("test.wav", true);

    delete[] origDownContent;
    return 0;
  }

  // FINAL CASES
  if (input.getAction() == ""){
    cout << "aborting: no action specified?"<< endl;
    return -1;
  }
  cout << "aborting: action not understood! " << input.getAction() << endl;
  return -1;


}










































  // // GENERATE D-LIST. needs:
  // // -a action
  // // -p project name
  // // -i iterations
  // if (input.getAction() == "generate_list"){

  //   // load metadata and extract important features
  //   const string metadata_path = input.getProjectPath()+METADATA_NAME;
  //   const string ANALYSIS_DIR = input.getProjectPath()+"ANALYSIS/";
  //   long int SAMPLEDOWN_RATIO = 0;
  //   unsigned int MAX_LEN = 0; // the size in samples of the biggest CC file
  //   double MAX_ENERGY = 0;
  //   vector<metadata> METADATA; // (wavPath, size, energy)
  //   ifstream inMeta(metadata_path.c_str());
  //   if(inMeta.is_open()==false){
  //     cout << "aborting: couldnt open " << metadata_path << endl;
  //     return 2; // RETURN 2: COULDN'T OPEN METADATA
  //   } else{
  //     string s;
  //     // load sampledown ratio (first line)
  //     inMeta>>s;
  //     int sepPos = s.find_first_of(":");
  //     SAMPLEDOWN_RATIO = stoi(s.substr(sepPos+1));
  //     while(inMeta >>s){
  //       int sepPos = s.find_first_of(':');
  //       string wavpath = s.substr(0, sepPos);
  //       string rest = s.substr(sepPos+1);
  //       sepPos = rest.find_first_of(':');
  //       unsigned int len = stoi(rest.substr(0, sepPos));
  //       double energy = stod(rest.substr(sepPos+1));
  //       METADATA.push_back(metadata{wavpath, len, energy});
  //       MAX_LEN = (len>MAX_LEN)? len : MAX_LEN;
  //       MAX_ENERGY = (energy>MAX_ENERGY)? energy : MAX_ENERGY;
  //     }
  //   }
  //   inMeta.close();
    
  //   // the wav2wav vector to be filled
  //   vector<d_ref> D; // d_ref {metadata meta; int m_id; int del; double k;}
    
  //   // prepare random generation:
  //   // a call to distribution(gen) should generate unif. ints between the second
  //   // and last idx of metadata (corresponding to the materials)
  //   // WARNING: idx 1 corresponds to m0!
  //   random_device rd;
  //   mt19937 gen(static_cast<mt19937::result_type>(time(nullptr)));
  //   uniform_int_distribution<> distribution(1, METADATA.size()-1);

  //   MAX_LEN = downsamplingLength(MAX_LEN, SAMPLEDOWN_RATIO);
  //   unsigned int TEMPSIG_LEN = 2*MAX_LEN; // so now we know zero is at tempSig[MAX_LEN]




  //   //////////////////////////////////////////////////////////////////////////////
  //   /// LOOP
  //   //////////////////////////////////////////////////////////////////////////////
  //   int counter = 0;
  //   for (unsigned int i=0; i<input.getIterations(); ++i){
  //     cout << "processing " << counter << "/" << input.getIterations() << endl;
  //     counter++;
  //     //for (int r : {1,2,0,0,1}){

  //     const int r = distribution(gen)-1;
  //     const unsigned int m_size = METADATA[r+1].size;
  //     const unsigned int mshift_len = (m_size-1)/SAMPLEDOWN_RATIO;
  //     // CALCULATE DOWNSAMPLED LENGTH
  //     if(m_size>METADATA[0].size){
  //       cout <<"ignoring "<< METADATA[r+1].wavPath <<": material longer than original!"<< endl;
  //     } else { // if chosen material isn't longer than original...
  //       // initialize local containers for iteration
  //       DoubleSignal* tempSig = new DoubleSignal(TEMPSIG_LEN); // zeros at beg.

  //       double maxVal = 0;//- numeric_limits<double>::infinity(); // initialize at -inf
  //       int maxPos = 0;


  //       // for each material in D...
  //       for(d_ref &d : D){ // wavPath, m_id, del, k
  //         // get the name of the corresponding CCMs
  //         pair<int, int> tup;
  //         if (d.m_id<r){
  //           tup.first = d.m_id;
  //           tup.second = r;
  //         } else {
  //           tup.first = r;
  //           tup.second = d.m_id;
  //         }
  //         string ccName = "cc_m"+to_string(tup.first)+"_m"+to_string(tup.second)+".wav";
  //         DoubleSignal ccm(ANALYSIS_DIR+ccName, false);
  //         if(r<d.m_id){ccm.reverse();} // ensure that our r is always the "shifting" sig
  //         for(int i=0; i<ccm.length(); ++i){
  //           tempSig->decrementAt(i+MAX_LEN+d.del-mshift_len, ccm[i]*d.k);
  //         }
  //         //tempSig->prettyPrint("tempSig after substracting "+ccName);
  //       }

  //       // now get the CCS and add it to tempSig
  //       string ccs_name = ANALYSIS_DIR+"cc_original_m"+to_string(r)+".wav";
  //       DoubleSignal ccs(ccs_name, false);
  //       for(int i=0; i<tempSig->length(); ++i){
  //         (*tempSig)[i] += ccs.at((i-MAX_LEN)+mshift_len, 0);
  //         if(abs((*tempSig)[i])>abs(maxVal)){
  //           maxVal = (*tempSig)[i];
  //           maxPos = i-MAX_LEN;
  //         }
  //       }
  //       //tempSig->prettyPrint("tempSig after adding ccs");

  //       // find maximum in tempSig, and add result to D
  //       double k_factor = maxVal * MAX_ENERGY / (METADATA[r+1].energy); //******************
  //       D.push_back(d_ref{METADATA[r+1], r, maxPos,k_factor});// maxPos, k_factor});
  //       // cout << i+1 << "/" << input.getIterations() << ") "<< "added signal " << r <<
  //       //   " with delay "  << maxPos << " and norm " << k_factor << endl << endl;
  //       // delete instantiated signals before finishing iteration
  //       delete tempSig;
  //     }
  //   }

  //   // SAVE D TO TXT FILE
  //   const string list_path = input.getProjectPath()+"reconstruction_"+
  //     to_string(input.getIterations()) + "iterations_" +
  //     to_string(SAMPLEDOWN_RATIO) + "ratio.txt";
  //   ofstream outFile(list_path, ios::out);
  //   if(!outFile.is_open()){
  //     cout << "aborting: couldnt open " << list_path << endl;
  //     return 3; // RETURN 3: COULDN'T MAKE D_LIST
  //   } else { // if able to open d_list
  //     cout << "writing " << list_path << endl;
  //     for(d_ref d : D){
  //       outFile << d.meta.wavPath << ":" << d.del*SAMPLEDOWN_RATIO << ":"
  //               << d.k << endl;
  //     }
  //     outFile.close();
  //     cout << "wrote "<< list_path << " succesfully!" << endl;
  //     return 0;
  //   }
  // }
 












  
  // // GENERATE WAV FILE FROM D-LIST. needs:
  // // -a action
  // // -p project name
  // // -d name of the d-list
  // if (input.getAction() == "generate_wav"){
    
  //   // this chunk is just to get the sampledownRatio and the original's metadata
  //   const string metadata_path = input.getProjectPath()+METADATA_NAME;
  //   // unsigned int SAMPLEDOWN_RATIO = 0;
  //   ifstream inMeta(metadata_path.c_str());
  //   if(inMeta.is_open()==false){
  //     cout << "aborting: couldnt open " << metadata_path << endl;
  //     return 2; // RETURN 2: COULDN'T OPEN METADATA
  //   }
  //   string s;
  //   // load the downsample ratio
  //   inMeta>>s; //do not comment out this! first line is sampledown, not original
  //   // int sepPos = s.find_first_of(":");
  //   // SAMPLEDOWN_RATIO = stoi(s.substr(sepPos+1));
  //   // now load the specs for the original
  //   inMeta >> s;
  //   int sepPos = s.find_first_of(':');
  //   const string ORIGINAL_PATH = s.substr(0, sepPos);
  //   string rest = s.substr(sepPos+1);
  //   sepPos = rest.find_first_of(':');

    
  //   const unsigned int ORIG_LEN = stoi(rest.substr(0, sepPos));
  //   inMeta.close();
    
  //   //
  //   DoubleSignal reconstruction(ORIG_LEN);
  //   reconstruction.setSFInfo(reconstruction.length(), 44100, 1, 65538, 1, 1);

    
  //   // load the D-list and write to reconstruction
  //   const string dlist_path = input.getProjectPath()+input.getDListName();
  //   vector<tuple<string, int, double>> D;
  //   ifstream inList(dlist_path.c_str());
  //   if(inList.is_open()==false){
  //     cout << "aborting: couldn't open " << dlist_path << endl;
  //     return 4; // RETURN 4: COULDN'T LOAD DLIST FILE
  //   }
  //   while(inList >>s){
  //     sepPos = s.find_first_of(':');
  //     string wavpath = s.substr(0, sepPos);
  //     string rest = s.substr(sepPos+1);
  //     sepPos = rest.find_first_of(':');
  //     int del = stoi(rest.substr(0, sepPos));
  //     double norm = stod(rest.substr(sepPos+1));
  //     DoubleSignal ds(wavpath, true); // ds is a material, NORMALIZED
  //     for (int i=0; i<reconstruction.length(); ++i){
  //       reconstruction[i] += ds.at(i, del)*norm; // del*SAMPLEDOWN_RATIO)*norm;
  //       //cout << "reconstruction: " << reconstruction[10000] << endl;
  //     }
  //   }
  //   inList.close();
  //   reconstruction.toWav(dlist_path+".wav", true);
  //   cout  << "toWav: succesfully saved to "<< dlist_path << ".wav!" << endl;
  //   return 0;
  // }



  //   // GENERATE CROSS-CORRELATIONS. needs:
  // // -a action
  // // -s originalName, WHOLE PATH
  // // -m mat1 mat2 ... WHOLE PATHS
  // // -r sampledown ratio
  // // -p project name
  // if (input.getAction() == "preprocess"){
  //   string p = input.getProjectPath();
  //   string cmd1 = "rm -rf "+p;
  //   string cmd2 = "mkdir "+p;
  //   string cmd3 = "mkdir "+p+"ANALYSIS";
  //   int response1 = system(cmd1.c_str());
  //   int response2 = system(cmd2.c_str());
  //   int response3 = system(cmd3.c_str());
  //   if (response1+response2+response3!=0){
  //     cout << "aborting: couldn't make directories" << p << endl;
  //     return 1;  // RETURN 1: COULDN'T MAKE DIRECTORIES
  //   } else{ // if folders could be created
  //     // make METADATA and CCs
  //     CrossCorrelator cc(p,
  //                        input.getOriginalPath(), input.getMaterialPaths(), METADATA_NAME,
  //                        input.getSampledownRatio());
  //     return 0; 
  //   }
  // }




//       cv::Mat testvec = cv::Mat(1, matSize, CV_32F, matContent);//(matContent);
