// EXAMPLE OF COMPILING AND RUNNING PROGRAM
// g++ -O3 -Wall -Wextra -pedantic -std=c++14  MAIN.cpp inputparser.cpp floatsignal.cpp wavsequence.cpp -o MAIN -Wl,-rpath,opencv-3.1.0  -lopencv_core -lopencv_imgproc -lopencv_ml -lsndfile && valgrind --leak-check=full -v ./MAIN -a test_cv -i 1000 -r 20


// std libs
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
//using namespace chrono;


////////////////////////////////////////////////////////////////////////////////////////////////////
/// TODO
////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPLEMENT PICK METHOD IN WAVSELECTOR
// add method to wavselector to load all wav files in a folder
// change g++ to load all cpp files
// add inputparser flag to load wavselector list



// implement some different optimization heuristic (split file into parts?) (wavselectorlist generator?)



////////////////////////////////////////////////////////////////////////////////////////////////////
/// CLI
////////////////////////////////////////////////////////////////////////////////////////////////////

// -a action
// -s originalName, WHOLE PATH
// -m mat1 mat2 ... WHOLE PATHS
// -r sampledown ratio
// -i iterations
// -d optimum list name
// -p pick list name


////////////////////////////////////////////////////////////////////////////////////////////////////
/// HELPING DS AND FUNCTIONS
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// MAIN ROUTINE
////////////////////////////////////////////////////////////////////////////////////////////////////
int PRINT_FREQ = 100;



int main(int argc, char **argv){
  InputParser input(argc, argv);

  // if (input.getAction()=="asdf"){
  //   WavSelector w("|", "#");
  //   w.add("anvil.wav", 1);
  //   w.add("anvil[-160].wav", 10);
  //   w.normalize();
  //   for (int i=0; i<50; ++i){
  //     w.pick() << endl;
  //   }
    
  //   // w.normalize();
  //   // w.normalize();
  //   // w.toFile("anodatest.txt");
  //   // cout << w.asString() << endl;
  //   return 0;
  // }

  
  if (input.getAction() == "test_cv"){
    // check that all needed parameters are present
    if (!input.getIterations() || !input.getSampledownRatio()){
      cout << "aborting: action " << input.getAction() << " requires -i <N> iterations, "
           << "and -r <M> sampledown ratio" << endl;
      return -1;
    }
    // load the rest of input parameters, parse optList and load original signal
    const int DS_RATIO = input.getSampledownRatio();
    WavSequence optList(input.getOptListName(), "|", "#");
    WavSelector pickList(input.getPickListName(), "|", "#");
    string origPath = "child-short.wav";
    FloatSignal original(origPath);
    float* origContent = original.getContent();
    SF_INFO* origInfo = original.getSFInfo();
    int origSize = original.getSize();
    // add contents of optList to original and downsample it before starting optimization
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
    for (unsigned int i=0; i<input.getIterations(); ++i){
      bool printFlag = i%PRINT_FREQ==0;
      if(printFlag){
        cout << "==iteration " << i << "/"<< input.getIterations() << "==" << endl;
      }
      // load the "material" and declare related variables
      string matPath = pickList.pick();
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
      double norm_factor_approx = cc[optPos_dw] / matDownEnergy;
      optList.add(optPos, norm_factor_approx, matPath);
      // update result & optimization references by substracting the optimized material
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
    // once the optimization loop is done, export optList and reconstruction
    delete[] origDownContent; // this was needed only for optimization
    string outName = "output";
    cout << "Optimization done. Saving .txt and .wav to " << outName << endl;
    FloatSignal reconstruction(origPath);
    float* reconsContent= reconstruction.getContent();
    int reconsSize = reconstruction.getSize();
    cout << "original energy is " << energy(reconsContent, reconsSize) << endl;
    for (int i=0; i<reconsSize; ++i){// ORIG - (ORIG-RECONSTRUCTION) = RECONSTRUCTION 
      reconsContent[i] -= origContent[i];
    }
    cout << "result energy is " << energy(reconsContent, reconsSize) << endl;
    reconstruction.toWav(outName+".wav", true);
    optList.toFile(outName+"_sequence.txt");
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
