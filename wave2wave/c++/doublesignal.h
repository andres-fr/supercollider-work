#ifndef DOUBLESIGNAL_H
#define DOUBLESIGNAL_H

// stl includes

// other libraries
#include <sndfile.hh>
#include "libalglib/ap.h"
#include "freefunctions.h"

using namespace std;



class DoubleSignal : public alglib::real_1d_array {
 private:
  int delIdx; // this is a register, to speed up the at() method
  SF_INFO* sfInfo;
 public:
  // constructors
  explicit DoubleSignal();
  explicit DoubleSignal(int size);
  explicit DoubleSignal(const string txtPath);
  explicit DoubleSignal(const string wavPath, const bool autoNorm);
  // deleted copy- and assignment constructors
  DoubleSignal(const DoubleSignal& copyFrom)=delete;
  DoubleSignal& operator= (const DoubleSignal& copyFrom)=delete;
  // destructor
  ~DoubleSignal();
  //setters
  void setSFInfo(const sf_count_t fr, const int sr,
               const int chans, const int formt,
                 const int sect, const int seek);
  void decrementAt(const int idx, const double val);
  void incrementAt(const int idx, const double val);
  // getters
  SF_INFO* getSFInfo() const;
  // special getter (returns contents[idx-delay], never throws error)
  double at(int idx, int del);
  // further functionality
  void printSFInfo();
  void prettyPrint(const string header);
  bool checkSRateAndChans(const SF_INFO* sf2);
  double energy();
  void multiplyBy(const double x);
  void reverse();
  // export
  void toWav(const string pathOut, const bool norm,
             const unsigned int downSampleRatio=1,
             const unsigned int offset=0);
  void toASCII(const string pathOut);
};

#endif
