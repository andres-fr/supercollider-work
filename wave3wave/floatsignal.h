#ifndef FLOATSIGNAL_H
#define FLOATSIGNAL_H

// stl includes

// other libraries
#include <sndfile.hh>

using namespace std;



class FloatSignal {
 private:
  //int delIdx; // this is a register, to speed up the at() method
  float* content;
  SF_INFO* sfInfo;
  int size;
  void normalize();
 public:
  // constructors
  explicit FloatSignal(const string wavPath, const bool norm);
  // deleted copy- and assignment constructors
  FloatSignal(const FloatSignal& copyFrom)=delete;
  FloatSignal& operator= (const FloatSignal& copyFrom)=delete;
  // destructor
  ~FloatSignal();
  // //setters
  // void setSFInfo(const sf_count_t fr, const int sr,
  //              const int chans, const int formt,
  //                const int sect, const int seek);
  // void decrementAt(const int idx, const double val);
  // void incrementAt(const int idx, const double val);
  // getters
  SF_INFO* getSFInfo() const;
  float* getContent();
  int getSize() const;
  // // special getter (returns contents[idx-delay], never throws error)
  // double at(int idx, int del);
  // further functionality
  // void printSFInfo();
  // void prettyPrint(const string header);
  // bool checkSRateAndChans(const SF_INFO* sf2);
  //float energy();
  // void multiplyBy(const double x);
  // void reverse();
  // // export
  void toWav(const string pathOut, const bool norm);
  // void toASCII(const string pathOut);
};

#endif
