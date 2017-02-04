// g++ -O3 -std=c++14 -Wall -Wextra -pedantic MAIN.cpp floatsignal.cpp crosscorrelator.cpp freefunctions.cpp -o MAIN -lsndfile -lalglib && valgrind --leak-check=yes ./MAIN

//#include <sndfile.hh>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <tuple>
#include <string>
// #include <cmath>
// #include <random>
// #include <chrono>
#include "doublesignal.h"
#include "crosscorrelator.h"

using namespace std;
//using namespace chrono;

// PROFILING:
// TODO: save raw in a more memory-efficient way:
// http://stackoverflow.com/questions/4643641/best-way-to-output-a-full-precision-double-into-a-text-file
// http://www.mega-nerd.com/libsndfile/api.html  0x1006=65542=wav=float32bit


// 1) load data as a FloatSignal
// 2) the crosscorrelator should output all CCs NORMALIZED to the max cc, this means:
//    a) compute all energies, and divide all CCs you store by the maximal energy found. This holds because no dot product can be higher than themaximal energy,which will be 1 after norm.
//    b) compute all CCs, store everything as wav float UNNORMALIZED file divided by the maximum peak
//    c) test: play max filename, play some others. Load them back to FloatSignal, check that the CC proportions hold!
// 3) Retry Algo: check space and time speedup

// DEBUGGING:
//  1) print energy of residual signal: check that it effectively decreases on each iteration
//  2) c


// TWEAKING:
// 1) calculate mean and stddev of the maxima of all CCSs, and discard the ones whose
//    maximum is below mean-stddev
//


// check that energy of metadata was calculated after normalization(0, 1)

struct metadata {string wavPath; int size; double energy;};
struct d_ref {metadata meta; int m_id; int del; double k;};




int main(){

  string AUDIO_DIR = "/home/afr/git/supercollider-work/wave2wave/AUDIO/";
  string ANALYSIS_DIR = "/home/afr/git/supercollider-work/wave2wave/ANALYSIS/";
  string OUTPUT_DIR = "/home/afr/git/supercollider-work/wave2wave/OUTPUT/";
  string METADATA_ADDRESS = ANALYSIS_DIR+"METADATA.txt";

  string orig = AUDIO_DIR+"child-short.wav";
  vector<string> mats;
  for (int i=-500; i<=500; i+=4){
    string tmp = AUDIO_DIR+"anvil["+to_string(i)+"].wav";
    mats.push_back(tmp);
  }
  CrossCorrelator cc(AUDIO_DIR+"child-short.wav", mats, OUTPUT_DIR);


  // DoubleSignal s1(AUDIO_DIR+"child-short.wav",true);
  // DoubleSignal s2(AUDIO_DIR+"anvil[-12].wav",true);
  // DoubleSignal s3(OUTPUT_DIR+"cc_m0_m0.wav",false);
  // cout << "original energy: " << s1.energy() <<endl;
  // cout << "material energy: " << s2.energy() <<endl;
  // cout << "cc outputted energy: " << s3.energy() <<endl;
  // cout << "energy(cc)*energy(orig)^2: " << s3.energy()*s1.energy()*s1.energy() <<endl;





// original energy: 3221.61
// material energy: 207.386
// cc outputted energy: 2.99341
// energy(cc)*energy(orig)^2: 3.1068e+07


  // float max = 0;
  // float min = 0;
  // float sum = 0;
  // for (int i=0; i<fs2.length(); ++i){
  //   float tmp = fs2.at(i,0);
  //   sum +=tmp;
  //   if (tmp>max){max = tmp;}
  //   if (tmp<min){min = tmp;}
  // }

  // cout << "max: " << max << endl;
  // cout << "min: " << min << endl;
  // cout << "sum: " << sum << endl;


  cout << "test finished"<< endl;
  return 0;
}
