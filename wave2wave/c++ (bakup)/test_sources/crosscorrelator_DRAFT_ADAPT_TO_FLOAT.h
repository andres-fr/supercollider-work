#ifndef FLOATSIGNAL_H
#define FLOATSIGNAL_H

// stl includes
//#include <vector>
#include <string>
// other libraries
#include <sndfile.hh>
// #include "libalglib/ap.h"
// #include "freefunctions.h"

using namespace std;


class FloatSignal {
private:
  float* arr;
  unsigned int size;
  SF_INFO* sfInfo;
public:
  // constructors and destructor
  explicit FloatSignal(const unsigned int sz);
  explicit FloatSignal(const string wavPath, const bool norm);
  // deleted copy- and assignment constructors
  FloatSignal(const FloatSignal& copyFrom)=delete;
  FloatSignal& operator= (const FloatSignal& copyFrom)=delete;
  // destructor
  ~FloatSignal();
  // setters
  void multiplyBy(float const x);
  void setSFInfo(const sf_count_t fr, const int sr,
                 const int chans, const int formt,
                 const int sect, const int seek);
  // getters
  unsigned int length() const;
  float* data() const;
  float readAt(const int idx) const;
  SF_INFO* getSFInfo() const;
  //
  float energy() const;
  //
  void toWav(const string pathOut, const bool norm);

};

#endif

// class FloatSignal : public alglib::real_1d_array {
//  private:
//   vector<float> data;
//   SF_INFO* sfInfo;
//  public:
//   // constructors
//   explicit FloatSignal();
//   explicit FLoatSignal(int size);
//   explicit FloatSignal(int size);
//   explicit FloatSignal(const string txtPath);
//   explicit FloatSignal(const string wavPath, const bool as);
//   // deleted copy- and assignment constructors
//   FloatSignal(const FloatSignal& copyFrom)=delete;
//   FloatSignal& operator= (const FloatSignal& copyFrom)=delete;
//   // destructor
//   ~FloatSignal();
//   //setters
//   void setSFInfo(const sf_count_t fr, const int sr,
//                const int chans, const int formt,
//                  const int sect, const int seek);
//   // getters
//   SF_INFO* getSFInfo() const;
//   // special getter (returns contents[idx-delay], never throws error)
//   double at(int idx);
//   // further functionality
//   void printSFInfo();
//   bool checkSRateAndChans(const SF_INFO* sf2);
//   double energy();
//   // export
//   void toNormalizedWav(const string pathOut);
//   void toASCII(const string pathOut);
// };

// #endif
