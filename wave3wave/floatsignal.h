#ifndef FLOATSIGNAL_H
#define FLOATSIGNAL_H

// stl includes

// other libraries
#include <sndfile.hh>

using namespace std;



class FloatSignal {
 private:
  float* content;
  SF_INFO* sfInfo;
  int size;
  void normalize(); // side effects!
 public:
  // constructor
  explicit FloatSignal(const string wavPath, const bool norm=true);
  // deleted copy- and assignment constructors
  FloatSignal(const FloatSignal& copyFrom)=delete;
  FloatSignal& operator= (const FloatSignal& copyFrom)=delete;
  // destructor
  ~FloatSignal();
  // getters
  SF_INFO* getSFInfo() const;
  float* getContent();
  int getSize() const;
  // further functionality
  float* getDownsampledCopy(const int ratio); // side effects!
  void toWav(const string pathOut, const bool norm);
};

#endif
