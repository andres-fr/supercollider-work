// EXAMPLE OF COMPILING RUNNING AND CHECKING A PROGRAM

// using a folder with materials:
// g++ -O3 -Wall -Wextra -pedantic -std=c++14 ./*.cpp -o MAIN -Wl,-rpath,opencv-3.1.0  -lopencv_core -lopencv_imgproc -lopencv_ml -lsndfile &&  valgrind --leak-check=full -v ./MAIN -s child-short.wav -i 500 -r 20 -m ./materials -f 100

// using a picklist
// g++ -O3 -Wall -Wextra -pedantic -std=c++14 ./*.cpp -o MAIN -Wl,-rpath,opencv-3.1.0  -lopencv_core -lopencv_imgproc -lopencv_ml -lsndfile && valgrind --leak-check=full -v ./MAIN -s child-short.wav -i 500 -r 20 -p picklist.txt -f 100


// std libs
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <tuple>
#include <algorithm>
#include <dirent.h>
// 3rd party libs
#include "sndfile.hh"
#include <opencv2/opencv.hpp>
// own dependencies
#include "inputparser.h"
#include "floatsignal.h"
#include "wavsequence.h"
#include "wavselector.h"
// namespacing
using namespace std;


////////////////////////////////////////////////////////////////////////////////////////////////////
/// TODO
////////////////////////////////////////////////////////////////////////////////////////////////////

// implement some heuristic to apply on the wavselector distribution



////////////////////////////////////////////////////////////////////////////////////////////////////
/// CLI
////////////////////////////////////////////////////////////////////////////////////////////////////

// -s path to original signal to be reconstructed                                          *REQUIRED
// -i number of iterations to perform (ind)                                                *REQUIRED
// -p path to ASCII material list holding <WEIGHT>|<MATERIAL_PATH> lines                   *ALT_1
// -m path to folder holding the .wav files to be used as materials                        *ALT_2
// -d path to ASCII sequence of <DELAY>|<NORM_RATIO>|<MATERIAL_PATH> to be loaded&expanded *OPTIONAL
// -a action to perform. Default="optimize"                                                *OPTIONAL
// -r sampledown ratio. Default=1. Higher ratios speed up but lose precision               *OPTIONAL
// -f the program will print a progress update each -f <N> iterations. Default disabled    *OPTIONAL




////////////////////////////////////////////////////////////////////////////////////////////////////
/// HELPING FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////

float* cv_crosscorrelate(float* orig, int origLen, float* mat, int matLen){
  // declare original
  IplImage* origCV = cvCreateImage(cvSize(origLen, 1), IPL_DEPTH_32F, 1); // last 1 is num of chans
  memcpy((float*)origCV->imageData, orig, origLen*sizeof(float));
  // declare material
  IplImage* matCV = cvCreateImage(cvSize(matLen, 1), IPL_DEPTH_32F, 1);
  memcpy((float*)matCV->imageData, mat, matLen*sizeof(float));
  // declare result
  int ccLen = origLen-matLen+1;
  IplImage* ccCV = cvCreateImage(cvSize(ccLen,1), IPL_DEPTH_32F, 1);
  // perform CC and copy result to float array
  cvMatchTemplate(origCV, matCV, ccCV, CV_TM_CCORR);
  float* cc = new float[ccLen]();
  memcpy(cc, (float*)ccCV->imageData, ccLen*sizeof(float));
  // release allocated memory and return
  cvReleaseImage(&origCV);
  cvReleaseImage(&matCV);
  cvReleaseImage(&ccCV);
  return cc;
}

double energy(float* arr, int size){return inner_product(arr, arr+size, arr, 0.0);}


vector<string> returnFilesInFolder(string path, const string extension=""){
  vector<string> files;
  vector<string> subFolders;
  DIR* pDir = opendir(path.c_str());
  dirent* entry;
  if(!pDir){
    throw invalid_argument("returnFilesInFolder: couldn't open path '"+path+
                           "' (Is it a valid folder path?)");
  } else{// if path was valid...
    if(path.back() != '/'){
      path += "/";
      cout << "WARNING: adding '/' to the end of the -m path! result: " << path << endl;
    }
    while((entry = readdir(pDir)))
      {
        if (entry->d_type==DT_REG){ // if entry is a file...
          string n = entry->d_name;
          if (n.find(extension, (n.length()-extension.length()))!=string::npos){
            files.push_back(path+n); // if entry has given extension add to list
          }
        }
        if(entry->d_type==DT_DIR){ // if entry is a directory...
          subFolders.push_back(entry->d_name); // add to list
        }
      }
    closedir(pDir);
  }
  return files; // subFolders
}


////////////////////////////////////////////////////////////////////////////////////////////////////
/// MAIN ROUTINE
////////////////////////////////////////////////////////////////////////////////////////////////////

// this defines the default syntax for the sequences managed by the algorithm
string DEFAULT_SEPARATOR = "|";
string DEFAULT_COMMENTMARK = "#";


int main(int argc, char **argv){

  InputParser input(argc, argv);

  // THE DEFAULT ACTION
  if (input.getAction() == "optimize"){
    // check that all needed parameters are present
    if (input.getOriginalPath().empty() || !input.getIterations()){
      cout << "aborting: action '" << input.getAction() << "' requires -s <PATH> to original .wav "
           << "file and -i <N> iterations!" << endl;
      return -1;
    }
    if(input.getPickListPath().empty() && input.getMaterialsPath().empty()){
      cout << "aborting: action '" << input.getAction() << "' requires either  -p <LIST_PATH> or "
           << "-m <FOLDER_PATH> to load the .wav materials for reconstruction!" << endl;
      return -1;
    }
    // load the rest of input parameters, parse optList and load original signal
    const int printFreq = input.getPrintFrequency();
    const int DS_RATIO = input.getSampledownRatio();
    WavSequence optList(input.getOptListPath(), DEFAULT_SEPARATOR, DEFAULT_COMMENTMARK);
    WavSelector* pickList; // has to be pointer because of conditional instantiation (see below)
    if(!input.getPickListPath().empty()){
      pickList = new WavSelector(input.getPickListPath(), DEFAULT_SEPARATOR, DEFAULT_COMMENTMARK);
    } else{
      pickList = new WavSelector(returnFilesInFolder(input.getMaterialsPath(), ".wav"));
    }
    string origPath = input.getOriginalPath();
    FloatSignal original(origPath);
    float* origContent = original.getContent();
    SF_INFO* origInfo = original.getSFInfo();
    int origSize = original.getSize();
    // add contents of optList (if any) to original
    for (wavsequence_entry s : optList.getContent()){
      FloatSignal mat(s.wavPath);
      float* matContent = mat.getContent();
      for (int i=0; i<mat.getSize(); ++i){
        int delIdx = i+s.delay;
        if(delIdx<0 || delIdx >= origSize){continue;}
        else{origContent[delIdx] -= matContent[i]*s.k;}
      }
    }
    // now the original can be downsampled for further optimization    
    int origDownSize = original.getSize()/DS_RATIO;
    float* origDownContent = original.getDownsampledCopy(DS_RATIO);

    // MAIN OPTIMIZATION LOOP
    cout << "==STARTING OPTIMIZATION:==" << endl;
    for (unsigned int i=1; i<=input.getIterations(); ++i){
      if(i%printFreq==0){
        cout << "==iteration " << i << "/"<< input.getIterations() << "==" << endl;
      }
      // load the "material" and declare related variables
      string matPath = pickList->pick();
      FloatSignal material(matPath);
      float* matContent = material.getContent();
      SF_INFO* matInfo = material.getSFInfo();
      int matSize = material.getSize();
      // check that the material is adequate
      if (matSize>origSize){
        cout << "WARNING: material " << matPath <<" is bigger than original "
             << origPath << ". Skipping" << endl;
        continue;
      }
      if (origInfo->samplerate!=matInfo->samplerate){
        cout << "WARNING: material " << matPath <<" must have same samplerate as original "
             << origPath << ". Skipping" << endl;
        continue;
      }
      if (origInfo->channels!=matInfo->channels){
        cout << "WARNING: material " << matPath <<" must have same number of channels as original "
             << origPath << ". Skipping" << endl;
        continue;
      }
      // if material is OK, prepare for CC
      int matDownSize = matSize/DS_RATIO;
      float* matDownContent = material.getDownsampledCopy(DS_RATIO);
      double matDownEnergy = energy(matDownContent, matDownSize);
      // calculate CC, extract optimization values and add them to optList
      float* cc = cv_crosscorrelate(origDownContent, origDownSize, matDownContent, matDownSize);
      auto minmax = minmax_element(cc, cc+origDownSize-matDownSize+1);
      int optPos_dw = ((abs(*minmax.first) > abs(*minmax.second))? minmax.first : minmax.second)-cc;
      int optPos = optPos_dw*DS_RATIO;
      double norm_factor_approx = cc[optPos_dw] / matDownEnergy; // approx because downsampling
      optList.add(optPos, norm_factor_approx, matPath);
      // update result & optimization references by subtracting the optimized material
      for (int i=0; i<matSize; ++i){
        int delIdx = i+optPos;
        if(delIdx<0 || delIdx >= origSize){continue;}
        else{ // subtract from the original array (will be useful at reconstruction)
          origContent[delIdx] -= matContent[i]*norm_factor_approx;
          if (i%DS_RATIO==0){ // subtract from the downsampled array (for further optimizations)
            int newI = i/DS_RATIO; // I hope this if guards are faster than a separate for loop
            if(newI<matDownSize){
              origDownContent[newI+optPos_dw] -= matDownContent[newI]*norm_factor_approx;
            }
          }
        }
      }
      // this arrays were valid just for one iteration, so delete them
      delete[] cc;
      delete[] matDownContent;
    }
    // once the optimization loop is done...
    // delete remaining garbage
    delete[] origDownContent;
    delete pickList;
    // calculate reconstruction with simple trick: ORIG-(ORIG-RECONSTRUCTION) = RECONSTRUCTION 
    FloatSignal reconstruction(origPath);
    float* reconsContent= reconstruction.getContent();
    int reconsSize = reconstruction.getSize();
    for (int i=0; i<reconsSize; ++i){
      reconsContent[i] -= origContent[i];
    }
    // export extended optList and reconstruction
    string outName =  origPath+"_reconstruction";
    cout << "Optimization done. Saving .txt and .wav to " << outName << endl;
    reconstruction.toWav(outName+".wav", true);
    optList.toFile(outName+"_sequence.txt");
    return 0;
  }

  
  // HANDLING OTHER ACTIONS
  if (input.getAction() == ""){
    cout << "aborting: no action specified?"<< endl;
    return -1;
  }
  cout << "aborting: action not understood! " << input.getAction() << endl;
  return -1; 
}
