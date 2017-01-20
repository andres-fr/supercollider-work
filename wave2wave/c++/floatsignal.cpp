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
FloatSignal::FloatSignal(int size, int delay){
  delay = delay;
  sz = size;
  contents = new float[sz]();
}

FloatSignal::~FloatSignal(){
  delete[] contents;
}

////////////////////////////////////////////////////////////////////////////////
// getters and setters
////////////////////////////////////////////////////////////////////////////////

int FloatSignal::getSize(){return sz;}
int FloatSignal::getDelay(){return delay;}
void FloatSignal::setDelay(int d){delay=d;}

float FloatSignal::at(int idx){
  int di = idx-delay;
  return (di<0 || di >= sz)? 0 : contents[di];
}

void FloatSignal::set(int idx, float val){
  if (idx>=0 && idx < sz){contents[idx] = val;}
  else {throw invalid_argument ("FloatSignal::set: index "+
                                to_string(idx)+" out of range!");}
}



////////////////////////////////////////////////////////////////////////////////
// further functionality
////////////////////////////////////////////////////////////////////////////////

void FloatSignal::print(){
  cout <<"FloatSignal: [";
  for (int i=0; i<(sz-1); i++){cout<<contents[i]<<", ";}
  cout <<contents[sz-1]<<"]"<<endl;
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
