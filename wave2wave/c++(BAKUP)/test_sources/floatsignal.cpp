#include <sndfile.hh>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include "floatsignal.h"
using namespace std;

// This file implements the FloatSignal class, an array of floats with some
// extra functionality.
//
// It has three private fields:
// -> float* contents: the pointer to the array created in the constructor
// -> int sz: the size of the created array,given at construction
// -> int delay: this is substracted from the index when reading the signal
//
// And the following public methods:
// -> setter/getter for delay, getter for sz
//
// -> void print(): a pretty print of the array's contents
// -> void normalize(float norm, float mean): adds and multiplies all the
//    array's elements, so that the maximum and the mean match the given ones

//
// The main reason for doing this class is the at method in combination with
// the delay field:
// -> float at(int idx): returns content at (idx-delay) if it is within bounds,
//    or zero else. This helps to code readability when adding signals with
//    different delay values: each signal remembers its own delay, and a single
//    iterator value passes through all of them.


////////////////////////////////////////////////////////////////////////////////
// constructors and destructor
////////////////////////////////////////////////////////////////////////////////

FloatSignal::FloatSignal(int size, float* conts){
  contents = conts;
  sz = size;
  delay = 0;
  sfInfo = new SF_INFO;
  sfInfo->format = 0;
}

FloatSignal::FloatSignal(string pathIn){
  sz = 0;
  delay = 0;
  sfInfo = new SF_INFO;
  sfInfo->format = 0;
  // declare container for filestream open path to file

  SNDFILE* infile = sf_open(pathIn.c_str(), SFM_READ, sfInfo);

  // check that file opens
  if(infile == nullptr){
    throw invalid_argument("FloatSignal: Unable to open input file: "+pathIn);
  }
  // configure auto-normalization settings
  if (!AUTO_NORMALIZE){
    sf_command(infile, SFC_SET_NORM_FLOAT, nullptr, SF_FALSE);
  }
  // fit size and contents to the incoming file
  sz = sfInfo->frames;
  contents = new float[sz]();
  sf_read_float(infile, contents, sz);
  sf_close(infile);
}


FloatSignal::~FloatSignal(){
  delete[] contents;
  delete sfInfo;
}

////////////////////////////////////////////////////////////////////////////////
// getters and setters
////////////////////////////////////////////////////////////////////////////////
// setters
void FloatSignal::setDelay(int d){delay=d;}
void FloatSignal::setSFInfo(SF_INFO sfi){sfInfo= &sfi;}
// getters
float * FloatSignal::getContents() const {return contents;}
int FloatSignal::getSize() const {return sz;}
int FloatSignal::getDelay() const {return delay;}
SF_INFO* FloatSignal::getSFInfo() const {return sfInfo;}
// special getter
float FloatSignal::at(int idx){
  int di = idx-delay;
  return (di<0 || di >= sz)? 0 : contents[di];
}

// void FloatSignal::set(int idx, float val){
//   if (idx>=0 && idx < sz){contents[idx] = val;}
//   else {throw invalid_argument ("FloatSignal::set: index "+
//                                 to_string(idx)+" out of range!");}
// }



////////////////////////////////////////////////////////////////////////////////
// further functionality
////////////////////////////////////////////////////////////////////////////////

void FloatSignal::print(){
  cout <<"FloatSignal: [";
  for (int i=0; i<(sz-1); i++){cout<<contents[i]<<", ";}
  cout <<contents[sz-1]<<"]"<<endl;
}

void FloatSignal::printSFInfo(){
  cout <<"sfInfo:"<<endl
       << "\rframes: " << sfInfo->frames << endl
       << "\rsamplerate: " << sfInfo->samplerate << endl
       << "\rchannels: " << sfInfo->channels << endl
       << "\rformat: " << sfInfo->format << endl
       << "\rsections: " << sfInfo->sections << endl
       << "\rseekable: " << sfInfo->seekable << endl;
}


void FloatSignal::normalize(float norm, float mean){
  int i=0;
  float sum = 0;
  float max = 0;
  // calculate mean into accum and get absolute maximum
  for(i=0; i<sz; i++){
    sum += contents[i];
    if (abs(contents[i]) > abs(max)){
      max = contents[i];
    }
  }
  float deltamean = (sum/sz)-mean;
  float deltanorm = norm / (max-deltamean);

  // substract accum to each sample, and renormalize
  for(i=0; i<sz; i++){
    contents[i] = (contents[i]-deltamean)*deltanorm;
  }
}


void FloatSignal::toWav(string pathOut){
  // check that SF_INFO was configured
  if(sfInfo == nullptr){
    throw invalid_argument("FloatSignal.toWav(): sfInfo not configured");
  }
  // declare and try to open outfile
  SNDFILE* outfile = nullptr;
  if((outfile = sf_open(pathOut.c_str(), SFM_WRITE, sfInfo)) == nullptr){
    throw invalid_argument("FloatSignal:Not able to open output file "+pathOut);
  }
  // configure auto-normalization settings
  if (!AUTO_NORMALIZE){
    sf_command(outfile, SFC_SET_NORM_FLOAT, nullptr, SF_FALSE);
  }
  // write signal contents (disregarding delay) to stream and close file
  sf_write_float(outfile, contents, sz);
  sf_close(outfile);
}
