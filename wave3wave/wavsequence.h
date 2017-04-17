#ifndef WAVSEQUENCE_H
#define WAVSEQUENCE_H

// stl includes
#include <vector>
// other libraries

// namespaces
using namespace std;


struct seq_entry{int delay; double k; string wavPath;};

class WavSequence {
 private:
  vector<seq_entry> content;
  string separator;
  string commentMarker;
 public:
  // constructors
  explicit WavSequence(const string sep, const string comm);
  explicit WavSequence(const string pathIn, const string sep,
                       const string comm);
  // deleted copy- and assignment constructors
  WavSequence(const WavSequence& copyFrom)=delete;
  WavSequence& operator= (const WavSequence& copyFrom)=delete;
  // destructor
  ~WavSequence();
  // //setters
  // void setSFInfo(const sf_count_t fr, const int sr,
  //              const int chans, const int formt,
  //                const int sect, const int seek);
  // void decrementAt(const int idx, const double val);
  // void incrementAt(const int idx, const double val);
  // getters
  /* SF_INFO* getSFInfo() const; */
  vector<seq_entry> getContent();
  string asString() const;
  /* float* getDownsampledCopy(const int ratio); */
  /* int getSize() const; */
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
  //void toWav(const string pathOut, const bool norm);
  void add(const int delay, const double k, const string wavPath);
  void toFile(const string pathOut);
};

#endif
