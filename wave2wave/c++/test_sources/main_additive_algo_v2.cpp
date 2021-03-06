// g++ -O3 -std=c++14 -Wall -Wextra -pedantic MAIN.cpp doublesignal.cpp crosscorrelator.cpp freefunctions.cpp -o MAIN -lsndfile -lalglib && valgrind --leak-check=yes ./MAIN

//#include <sndfile.hh>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include <tuple>
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


// check that energy of metadata was calculated after normalization(0, 1)

struct metadata {string wavPath; int size; double energy;};
struct d_ref {metadata meta; int m_id; int del; double k;};

int main(){

  string AUDIO_DIR = "/home/afr/git/supercollider-work/wave2wave/AUDIO/";
  string ANALYSIS_DIR = "/home/afr/git/supercollider-work/wave2wave/ANALYSIS/";
  string OUTPUT_DIR = "/home/afr/git/supercollider-work/wave2wave/OUTPUT/";
  string METADATA_ADDRESS = ANALYSIS_DIR+"METADATA.txt";

  int LOOP_SIZE = 50;



  // declare and load METADATA: idx 0 is original, rest are materials
  int MAX_LEN = 0; // the size in samples of the biggest CC file
  vector<metadata> METADATA; // (wavPath, size, energy)
  ifstream inSpecs(METADATA_ADDRESS.c_str());
  if(inSpecs.is_open()==false){
    cout << "couldnt open " << METADATA_ADDRESS << endl;
  } else{
    string s;
    while(inSpecs >>s){
      int sepPos = s.find_first_of(':');
      string wavpath = s.substr(0, sepPos);
      string rest = s.substr(sepPos+1);
      sepPos = rest.find_first_of(':');
      int len = stoi(rest.substr(0, sepPos));
      double energy = stod(rest.substr(sepPos+1));
      METADATA.push_back(metadata{wavpath, len, energy});
      MAX_LEN = (len>MAX_LEN)? len : MAX_LEN;
    }
  }


  // prepare random generation:
  // a call to distribution(gen) should generate unif. ints between the second
  // and last idx of metadata (corresponding to the materials)
  // WARNING: idx 1 corresponds to m0!
  random_device rd;
  mt19937 gen(static_cast<mt19937::result_type>(time(nullptr)));
  uniform_int_distribution<> distribution(1, METADATA.size()-1);


  // in this second version of the algo, each iteration loads its needed
  // files, and disposes them after
  vector<d_ref> D;

  for (int i=0; i<LOOP_SIZE; i++){
    int r = distribution(gen)-1;
    // load ccs
    DoubleSignal ccs(ANALYSIS_DIR+"cc_original_m"+to_string(r)+".txt");

    // load D-elements to ccm map
    map<string, DoubleSignal*> ccm;
    for(d_ref d : D){ //d_ref meta, m_id, del, k // metadata: wPath, size, energy
      string ccName = (d.m_id<r)? to_string(d.m_id)+"_m"+to_string(r)+".txt"
        : to_string(r)+"_m"+to_string(d.m_id)+".txt";
      ccName = ANALYSIS_DIR+"cc_m"+ccName;
      ccm.insert(make_pair(ccName, new DoubleSignal(ccName)));
    }
    // now we loaded all required cross-correlations.
    // initialize signal: the size of the maximal CC we can get
    DoubleSignal* tempSig = new DoubleSignal(2*MAX_LEN-1);
    // initialize containers to add to new element in D
    double maxVal = - numeric_limits<double>::infinity(); // initialize at -inf
    int maxPos = 0;
    // calculate tempSig = ccs-sum_all(ccm), in order to get optimal delay and k
    for(d_ref &d : D){ // wavPath, m_id, del, k
      string ccName = (d.m_id<r)? "m"+to_string(d.m_id)+"_m"+to_string(r)+".txt"
        : "m"+to_string(r)+"_m"+to_string(d.m_id)+".txt";
      ccName = ANALYSIS_DIR+ccName;
      for(int i=0; i<tempSig->length(); ++i){
        (*tempSig)[i] -= ccm[ccName]->at(i-MAX_LEN+d.meta.size-d.del);
      }
    }

    for(int i=0; i<tempSig->length(); ++i){
      (*tempSig)[i] += ccs.at(i-MAX_LEN+(METADATA[r+1].size));
      if((*tempSig)[i]>maxVal){
        maxVal = (*tempSig)[i];
        maxPos = i;
      }
    }
    // maxpos has the "ith" position: adapt it to current d
    //maxPos = maxPos-MAX_LEN+(METADATA[r+1].size);
    // add result to D and print
    D.push_back(d_ref{METADATA[r+1], r, maxPos, maxVal/(METADATA[r+1].energy)});
    cout << "added signal " << r << " with delay "  << maxPos << " and norm " << maxVal/(METADATA[r+1].energy) << endl;
    // delete instantiated signals before finishing iteration
    for(auto &x : ccm){
      delete x.second;
    }
    delete tempSig;

  }


  // RECONSTRUCT SIGNAL (inefficient: better do it with inverse CCs!!)
  DoubleSignal* reconstruction = new DoubleSignal(2*MAX_LEN-1);
  reconstruction->setSFInfo(2*MAX_LEN-1, 44100, 1, 65538, 1, 1);
  // substract ccms from tempsig
  for (d_ref d : D){ //d_ref meta, m_id, del, k // metadata: wPath, size, energy
    DoubleSignal ds(d.meta.wavPath, true);
    for (int i=0; i<(2*MAX_LEN-1); ++i){
      (*reconstruction)[i] += (ds.at(i-d.del)) * d.k;
    }
  }
  reconstruction->toNormalizedWav("reconstruction.wav");
  delete reconstruction;


  cout << "test finished"<< endl;
  return 0;
}
