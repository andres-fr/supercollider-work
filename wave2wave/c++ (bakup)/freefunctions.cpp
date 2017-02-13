// stl includes
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdexcept>
#include <string>
#include <algorithm>
// other libraries

// own dependencies

// own header
#include "freefunctions.h"
// namespaces
using namespace std;


// void normalizeArray(double* contents, const int sz, const double mean,
//                     const double maxVal){
//   //cout << "normalizing"<< endl;
//   int i=0;
//   double sum = 0;
//   double max = 0;
//   // calculate mean into accum and get absolute maximum
//   for(i=0; i<sz; i++){
//     double tmp = contents[i];
//     sum += tmp;
//     if (abs(tmp) > abs(max)){
//       max = tmp;
//     }
//   }
//   double deltamean = (sum/sz)-mean;
//   double deltanorm = abs(maxVal / (max-deltamean));
//   // substract accum to each sample, and renormalize
//   for(i=0; i<sz; i++){
//     contents[i] = (contents[i]-deltamean)*deltanorm;
//   }
// }


void normalizeArray(double* contents, const int sz){
  //cout << "normalizing"<< endl;
  int i=0;
  double sum = 0;
  double max = 0;
  double min = 0;
  // calculate mean into accum and get absolute maximum
  for(i=0; i<sz; i++){
    sum += contents[i];
    if (contents[i] > max){
      max = contents[i];
    } else if (contents[i] < min){
      min = contents[i];
    }
  }
  double mean = sum/sz;
  if (max==mean){
    cout << "normalizeArray: normalization not possible when max==mean" << endl;
  } else {
    max -= mean;
    min -= mean;
    double deltanorm = (abs(max)>abs(min))? abs(max) : abs(min);
    // substract accum to each sample, and renormalize
    for(i=0; i<sz; i++){
      contents[i] = (contents[i]-mean)/deltanorm;
    }
  }
}


void rotateArray(double* contents, const int size, const int pivot){
  //cout << "rotating array..." << endl;
  reverse(contents, contents+size);
  reverse(contents, contents+size-pivot);
  reverse(contents+size-pivot, contents+size);
}



void array2txt(double* contents, const int size, const string pathOut){
  ofstream outFile(pathOut.c_str(), ios::out);
  if (!outFile.is_open()){
    cout << "array2txt: couldn't open " << pathOut << endl;
    return;
  } else {// if file opened succesfully...
    const int PACKET_SIZE = 1000;
    int globalIter = 0;
    while(globalIter<(size-1)){
      string bufStr = "";
      int bufIter = 0;
      while(bufIter<PACKET_SIZE && globalIter<(size-1)){
        bufStr += to_string(contents[globalIter])+"\n";
        bufIter++;
        globalIter++;
      }
      outFile << bufStr;
    }
    outFile << contents[globalIter];
    outFile.close();
    cout << "array2txt: succesfully saved to " << pathOut << endl;
  }
}


unsigned int downsamplingLength(const unsigned int origLength,
                                const unsigned int downSampleRatio){
  if (downSampleRatio==1){ // in case no downsampling
    return origLength;
  } else if (downSampleRatio>1){ // in case valid downsampling
      unsigned int result = origLength/downSampleRatio;
      if (origLength%downSampleRatio > 0){
        return origLength/downSampleRatio+1;
      }
      else {
        return origLength/downSampleRatio;
      }
      return result;
  }
  else { // in case invalid downsampling
    cout << "downsamplingLength ERROR: ratio must be positive integer and was"
         <<  downSampleRatio << endl;
    return 0;
  }
}
