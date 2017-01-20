#ifndef WAVCLIENT_H
#define WAVCLIENT_H

#include <vector>
using namespace std;

class WavClient {
 private:
  static const int BLOCK_SIZE = 32;
  static const int SAMPLE_RATE = 44100;
  static const int NUM_CHANNELS = 1;
  static const int FORMAT = 65538;
 public:
  // constructors and destructor
  explicit WavClient(){};
  WavClient(const WavClient& copyFrom)=delete;
  WavClient& operator= (const WavClient& copyFrom)=delete;
  ~WavClient(){};
  // static functionality
  vector<float> wavPath2floatVector(string pathIn);
  void floatVector2wav(string pathOut, const vector<float> &wavRaw);
};

#endif
