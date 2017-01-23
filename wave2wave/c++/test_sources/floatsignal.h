#ifndef FLOATSIGNAL_H
#define FLOATSIGNAL_H


#include <sndfile.hh>

using namespace std;

class FloatSignal {

 private:
  // signal fields
  float* contents;
  int sz;
  int delay;
  // wav client fields
  SF_INFO* sfInfo;
  static const int BLOCK_SIZE = 32;
  static const int AUTO_NORMALIZE = true;

 public:
  // constructors and destructor
  explicit FloatSignal(int size) : FloatSignal(size, new float[size]()){};
  explicit FloatSignal(int size, float* conts);
  explicit FloatSignal(string wavPath);
  FloatSignal(const FloatSignal& copyFrom)=delete;
  FloatSignal& operator= (const FloatSignal& copyFrom)=delete;
  ~FloatSignal();
  //setters
  void setDelay(int d);
  void setSFInfo(SF_INFO sfi);
  // getters
  float* getContents() const;
  int getSize() const;
  int getDelay() const;
  SF_INFO* getSFInfo() const;
  // special getter (returns contents[idx-delay], never throws error)
  float at(int idx);

  // further functionality
  void normalize(float norm, float mean);
  void print(); // not affected by the delay
  void printSFInfo();
  void toWav(string pathOut);
};

#endif
