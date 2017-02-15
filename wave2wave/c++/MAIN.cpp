// g++ -O3 -std=c++14 -Wall -Wextra -pedantic MAIN.cpp doublesignal.cpp crosscorrelator.cpp freefunctions.cpp -o MAIN -lsndfile -lalglib && valgrind --leak-check=yes ./MAIN

// EXAMPLE OF GENERATING CCS
// g++ -O4 -std=c++14 -Wall -Wextra -pedantic MAIN.cpp doublesignal.cpp crosscorrelator.cpp freefunctions.cpp inputparser.cpp -o MAIN -lsndfile -lalglib && ./MAIN -a preprocess -s /home/afr/IMPULS/WORKSPACE/AUDIO/child-short.wav -m /home/afr/IMPULS/WORKSPACE/AUDIO/anvil[0].wav /home/afr/IMPULS/WORKSPACE/AUDIO/anvil[40].wav /home/afr/IMPULS/WORKSPACE/AUDIO/anvil[-40].wav -r 2 -p test_child

// EXAMPLE OF GENERATING D-LIST
// g++ -O4 -std=c++14 -Wall -Wextra -pedantic MAIN.cpp doublesignal.cpp crosscorrelator.cpp freefunctions.cpp inputparser.cpp -o MAIN -lsndfile -lalglib && ./MAIN -a generate_list -p test_child -i 100


// EXAMPLE OF GENERATING WAV
// ./MAIN -a generate_wav -p test_child -d reconstruction_1000iterations_2ratio.txt

//#include <sndfile.hh>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <tuple>

// #include <cmath>
// #include <random>
// #include <chrono>
#include "doublesignal.h"
#include "crosscorrelator.h"
#include "inputparser.h"

using namespace std;
//using namespace chrono;



// REPORT:
// talk about the command-line interface of the program
// report some real-case profiling
// play example: record stuff as materials


// PROFILING:
// develop heuristic to split a file in parts? (sitting in a room f.e.)




// -a action
// -s originalName, WHOLE PATH
// -m mat1 mat2 ... WHOLE PATHS
// -r sampledown ratio
// -i iterations
// -p project name
// -d the D-list



struct metadata {string wavPath; unsigned int size; double energy;};
struct d_ref {metadata meta; int m_id; int del; double k;};

int main(int argc, char **argv){
  const string WORKSPACE_DIR = "/home/afr/IMPULS/WORKSPACE/";
  const string METADATA_NAME = "METADATA.txt";
  InputParser input(argc, argv, WORKSPACE_DIR);


  // GENERATE CROSS-CORRELATIONS. needs:
  // -a action
  // -s originalName, WHOLE PATH
  // -m mat1 mat2 ... WHOLE PATHS
  // -r sampledown ratio
  // -p project name
  if (input.getAction() == "preprocess"){
    string p = input.getProjectPath();
    string cmd1 = "rm -rf "+p;
    string cmd2 = "mkdir "+p;
    string cmd3 = "mkdir "+p+"ANALYSIS";
    int response1 = system(cmd1.c_str());
    int response2 = system(cmd2.c_str());
    int response3 = system(cmd3.c_str());
    if (response1+response2+response3!=0){
      cout << "aborting: couldn't make directories" << p << endl;
      return 1;  // RETURN 1: COULDN'T MAKE DIRECTORIES
    } else{ // if folders could be created
      // make METADATA and CCs
      CrossCorrelator cc(WORKSPACE_DIR, p,
                         input.getOriginalPath(), input.getMaterialPaths(), METADATA_NAME,
                         input.getSampledownRatio());
      return 0; 
    }
  }



  // GENERATE WAV FILE FROM D-LIST. needs:
  // -a action
  // -p project name
  // -d name of the d-list
  if (input.getAction() == "generate_wav"){
    
    // this chunk is just to get the sampledownRatio and the original's metadata
    const string metadata_path = input.getProjectPath()+METADATA_NAME;
    // unsigned int SAMPLEDOWN_RATIO = 0;
    ifstream inMeta(metadata_path.c_str());
    if(inMeta.is_open()==false){
      cout << "aborting: couldnt open " << metadata_path << endl;
      return 2; // RETURN 2: COULDN'T OPEN METADATA
    }
    string s;
    // load the downsample ratio
    inMeta>>s; //do not comment out this! first line is sampledown, not original
    // int sepPos = s.find_first_of(":");
    // SAMPLEDOWN_RATIO = stoi(s.substr(sepPos+1));
    // now load the specs for the original
    inMeta >> s;
    int sepPos = s.find_first_of(':');
    const string ORIGINAL_PATH = s.substr(0, sepPos);
    string rest = s.substr(sepPos+1);
    sepPos = rest.find_first_of(':');
    const unsigned int ORIG_LEN = stoi(rest.substr(0, sepPos));
    inMeta.close();
    
    //
    DoubleSignal reconstruction(ORIG_LEN);
    reconstruction.setSFInfo(reconstruction.length(), 44100, 1, 65538, 1, 1);

    
    // load the D-list and write to reconstruction
    const string dlist_path = input.getProjectPath()+input.getDListName();
    vector<tuple<string, unsigned int, double>> D;
    ifstream inList(dlist_path.c_str());
    if(inList.is_open()==false){
      cout << "aborting: couldn't open " << dlist_path << endl;
      return 4; // RETURN 4: COULDN'T LOAD DLIST FILE
    }
    while(inList >>s){
      sepPos = s.find_first_of(':');
      string wavpath = s.substr(0, sepPos);
      string rest = s.substr(sepPos+1);
      sepPos = rest.find_first_of(':');
      unsigned int del = stoi(rest.substr(0, sepPos));
      double norm = stod(rest.substr(sepPos+1));
      DoubleSignal ds(wavpath, true); // ds is a material, NORMALIZED
      for (int i=0; i<reconstruction.length(); ++i){
        reconstruction[i] += ds.at(i, del)*norm;// del*SAMPLEDOWN_RATIO)*norm; //******************
        //cout << "reconstruction: " << reconstruction[10000] << endl;
      }
    }
    inList.close();
    reconstruction.toWav(dlist_path+".wav", true);
    return 0;
  }

    







  
  // GENERATE D-LIST. needs:
  // -a action
  // -p project name
  // -i iterations
  if (input.getAction() == "generate_list"){

    // load metadata and extract important features
    const string metadata_path = input.getProjectPath()+METADATA_NAME;
    const string ANALYSIS_DIR = input.getProjectPath()+"ANALYSIS/";
    unsigned int SAMPLEDOWN_RATIO = 0;
    unsigned int MAX_LEN = 0; // the size in samples of the biggest CC file
    double MAX_ENERGY = 0;
    vector<metadata> METADATA; // (wavPath, size, energy)
    ifstream inMeta(metadata_path.c_str());
    if(inMeta.is_open()==false){
      cout << "aborting: couldnt open " << metadata_path << endl;
      return 2; // RETURN 2: COULDN'T OPEN METADATA
    } else{
      string s;
      // load sampledown ratio (first line)
      inMeta>>s;
      int sepPos = s.find_first_of(":");
      SAMPLEDOWN_RATIO = stoi(s.substr(sepPos+1));
      while(inMeta >>s){
        int sepPos = s.find_first_of(':');
        string wavpath = s.substr(0, sepPos);
        string rest = s.substr(sepPos+1);
        sepPos = rest.find_first_of(':');
        unsigned int len = stoi(rest.substr(0, sepPos));
        double energy = stod(rest.substr(sepPos+1));
        METADATA.push_back(metadata{wavpath, len, energy});
        MAX_LEN = (len>MAX_LEN)? len : MAX_LEN;
        MAX_ENERGY = (energy>MAX_ENERGY)? energy : MAX_ENERGY;
      }
    }
    inMeta.close();
    
    // the wav2wav vector to be filled
    vector<d_ref> D; // d_ref {metadata meta; int m_id; int del; double k;}
    
    // prepare random generation:
    // a call to distribution(gen) should generate unif. ints between the second
    // and last idx of metadata (corresponding to the materials)
    // WARNING: idx 1 corresponds to m0!
    random_device rd;
    mt19937 gen(static_cast<mt19937::result_type>(time(nullptr)));
    uniform_int_distribution<> distribution(1, METADATA.size()-1);

    MAX_LEN = downsamplingLength(MAX_LEN, SAMPLEDOWN_RATIO);
    unsigned int TEMPSIG_LEN = 2*MAX_LEN; // so now we know zero is at tempSig[MAX_LEN]




    //////////////////////////////////////////////////////////////////////////////
    /// LOOP
    //////////////////////////////////////////////////////////////////////////////
    int counter = 0;
    for (unsigned int i=0; i<input.getIterations(); ++i){
      cout << "processing " << counter << "/" << input.getIterations() << endl;
      counter++;
      //for (int r : {1,2,0,0,1}){

      const int r = distribution(gen)-1;
      const unsigned int m_size = METADATA[r+1].size;
      const unsigned int mshift_len = (m_size-1)/SAMPLEDOWN_RATIO;
      // CALCULATE DOWNSAMPLED LENGTH
      if(m_size>METADATA[0].size){
        cout <<"ignoring "<< METADATA[r+1].wavPath <<": material longer than original!"<< endl;
      } else { // if chosen material isn't longer than original...
        // initialize local containers for iteration
        DoubleSignal* tempSig = new DoubleSignal(TEMPSIG_LEN); // zeros at beg.

        double maxVal = 0;//- numeric_limits<double>::infinity(); // initialize at -inf
        int maxPos = 0;


        // for each material in D...
        for(d_ref &d : D){ // wavPath, m_id, del, k
          // get the name of the corresponding CCMs
          pair<int, int> tup;
          if (d.m_id<r){
            tup.first = d.m_id;
            tup.second = r;
          } else {
            tup.first = r;
            tup.second = d.m_id;
          }
          string ccName = "cc_m"+to_string(tup.first)+"_m"+to_string(tup.second)+".wav";
          DoubleSignal ccm(ANALYSIS_DIR+ccName, false);
          if(r<d.m_id){ccm.reverse();} // ensure that our r is always the "shifting" sig
          for(int i=0; i<ccm.length(); ++i){
            tempSig->decrementAt(i+MAX_LEN+d.del-mshift_len, ccm[i]*d.k);
          }
          //tempSig->prettyPrint("tempSig after substracting "+ccName);
        }

        // now get the CCS and add it to tempSig
        string ccs_name = ANALYSIS_DIR+"cc_original_m"+to_string(r)+".wav";
        DoubleSignal ccs(ccs_name, false);
        for(int i=0; i<tempSig->length(); ++i){
          (*tempSig)[i] += ccs.at((i-MAX_LEN)+mshift_len, 0);
          if(abs((*tempSig)[i])>abs(maxVal)){
            maxVal = (*tempSig)[i];
            maxPos = i-MAX_LEN;
          }
        }
        //tempSig->prettyPrint("tempSig after adding ccs");


        // find maximum in tempSig, and add result to D
        double k_factor = maxVal * MAX_ENERGY / (METADATA[r+1].energy); //******************
        D.push_back(d_ref{METADATA[r+1], r, maxPos,k_factor});// maxPos, k_factor});
        // cout << i+1 << "/" << input.getIterations() << ") "<< "added signal " << r <<
        //   " with delay "  << maxPos << " and norm " << k_factor << endl << endl;
        // delete instantiated signals before finishing iteration
        delete tempSig;
      }
    }

    // SAVE D TO TXT FILE
    const string list_path = input.getProjectPath()+"reconstruction_"+
      to_string(input.getIterations()) + "iterations_" +
      to_string(SAMPLEDOWN_RATIO) + "ratio.txt";
    ofstream outFile(list_path, ios::out);
    if(!outFile.is_open()){
      cout << "aborting: couldnt open " << list_path << endl;
      return 3; // RETURN 3: COULDN'T MAKE D_LIST
    } else { // if able to open d_list
      cout << "writing " << list_path << endl;
      for(d_ref d : D){
        outFile << d.meta.wavPath << ":" << d.del*SAMPLEDOWN_RATIO << ":"
                << d.k << endl;
      }
      outFile.close();
      cout << "wrote "<< list_path << " succesfully!" << endl;
      return 0;
    }
  }
 





  // FINAL CASES
  if (input.getAction() == ""){
    cout << "aborting: no action specified?"<< endl;
    return 5; // RETURN 5: NO ACTION SPECIFIED
  }
  cout << "aborting: action not understood! " << input.getAction() << endl;
  return 6; // RETURN 6: ACTION NOT UNDERSTOOD


}
