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
  int delay;
  int delIdx; // this is a register, to speed up the at() method
  SF_INFO* sfInfo;
 public:
  // constructors
  explicit DoubleSignal();
  explicit DoubleSignal(const string txtPath);
  explicit DoubleSignal(const string wavPath, const bool as);
  // deleted copy- and assignment constructors
  DoubleSignal(const DoubleSignal& copyFrom)=delete;
  DoubleSignal& operator= (const DoubleSignal& copyFrom)=delete;
  // destructor
  ~DoubleSignal();
  //setters
  void setDelay(int d);
  void setSFInfo(const sf_count_t fr, const int sr,
               const int chans, const int formt,
                 const int sect, const int seek);
  // getters
  int getDelay() const;
  SF_INFO* getSFInfo() const;
  // // special getter (returns contents[idx-delay], never throws error)
  double at(int idx);
  // // further functionality
  void printSFInfo();
  void toWav(const string pathOut, const bool normalize);
  bool checkSRateAndChans(const SF_INFO* sf2);
  void toRaw(const string pathOut);
};

#endif
