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


// DEBUGGING:
//  1) print energy of residual signal: check that it effectively decreases on each iteration
//  2) run this: multiple bugs: the delta seems to substract perfectly only in the first step. why??
//  also, when all become negative, the zero becomes the maximum and its "optimized down" further,
//  WE DONT WANT THE MAXIMUM PER SE, BUT THE MAXIMUM ABSOLUTE??


// TWEAKING:
// 1) calculate mean and stddev of the maxima of all CCSs, and discard the ones whose
//    maximum is below mean-stddev
//


// check that energy of metadata was calculated after normalization(0, 1)

struct metadata {string wavPath; int size; double energy;};
struct d_ref {metadata meta; int m_id; int del; double k;};

int main(){

  string AUDIO_DIR = "/home/afr/git/supercollider-work/wave2wave/AUDIO/";
  //string ANALYSIS_DIR = "/home/afr/git/supercollider-work/wave2wave/ANALYSIS/";
  string OUTPUT_DIR = "/home/afr/git/supercollider-work/wave2wave/OUTPUT/";
  string METADATA_ADDRESS = OUTPUT_DIR+"METADATA.txt";

  CrossCorrelator cc(AUDIO_DIR+"child-short.wav", vector<string>{"test_m1.wav", "test_m2.wav"}, OUTPUT_DIR);




  int LOOP_SIZE = 0;
  cout << "type number of iterations: ";
  cin >> LOOP_SIZE;



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

  int ORIG_LEN = METADATA[0].size;


  // prepare random generation:
  // a call to distribution(gen) should generate unif. ints between the second
  // and last idx of metadata (corresponding to the materials)
  // WARNING: idx 1 corresponds to m0!
  random_device rd;
  mt19937 gen(static_cast<mt19937::result_type>(time(nullptr)));
  uniform_int_distribution<> distribution(1, METADATA.size()-1);


  // the wav2wav vector to be filled
  vector<d_ref> D; // d_ref {metadata meta; int m_id; int del; double k;}

  for (int i=0; i<LOOP_SIZE; ++i){
    int r = 0;// distribution(gen)-1;  //*******************
    // load ccs
    DoubleSignal ccs(OUTPUT_DIR+"cc_original_m"+to_string(r)+".wav", false);

    ccs.prettyPrint("CCS_"+to_string(r));//********************


    // initialize containers for iteration
    DoubleSignal* tempSig = new DoubleSignal(ORIG_LEN); // zeros at beg.
    double maxVal = 0;//- numeric_limits<double>::infinity(); // initialize at -inf
    int maxPos = 0;
    //calculate tempSig = ccs-sum_all(ccm), in order to get maxVal and maxPos
    for(d_ref &d : D){ // wavPath, m_id, del, k
      pair<int, int> tup;
      if (d.m_id<r){
        tup.first = d.m_id;
        tup.second = r;
      } else {
        tup.first = r;
        tup.second = d.m_id;
      }
      string ccName = "cc_m"+to_string(tup.first)+"_m"+to_string(tup.second)+".wav";
      DoubleSignal m(OUTPUT_DIR+ccName, false);



      for(int i=0; i<tempSig->length(); ++i){
        //cout << "substracting " << (*tempSig)[i] << " minus "<< m.at(i+((METADATA[tup.first+1].size)-1), d.del) * 7.0378 * d.k << endl; //***************


        (*tempSig)[i] -= m.at(i+((METADATA[tup.first+1].size)-1), d.del) * 7.0378 * d.k;
      }
    }
    for(int i=0; i<tempSig->length(); ++i){ //********
      (*tempSig)[i] += ccs.at(i+((METADATA[r+1].size)-1), 0);
      if(abs((*tempSig)[i])>abs(maxVal)){
        maxVal = (*tempSig)[i];
        maxPos = i;
      }
    }
    //tempSig->prettyPrint("tempSig: ");//********************


    // maxpos has the "ith" position: adapt it to current d
    //maxPos = maxPos-MAX_LEN+(METADATA[r+1].size);
    // add result to D and print
    double k_factor = maxVal/(METADATA[r+1].energy);
    D.push_back(d_ref{METADATA[r+1], r, maxPos, k_factor});
    cout << "added signal " << r << " with delay "  << maxPos << " and norm "
         << k_factor << endl << endl;
    // delete instantiated signals before finishing iteration
    delete tempSig;



  }

  // RECONSTRUCT SIGNAL (inefficient: better do it with inverse CCs!!)
  DoubleSignal reconstruction(ORIG_LEN);
  reconstruction.setSFInfo(ORIG_LEN, 44100, 1, 65538, 1, 1);
  for (d_ref d : D){ //d_ref meta, m_id, del, k // metadata: wPath, size, energy
    DoubleSignal ds(d.meta.wavPath, true); // ds is a material
    for (int i=0; i<reconstruction.length(); ++i){
      reconstruction[i] += (ds.at(i, d.del))*d.k;
    }

    // DoubleSignal original("test_orig.wav",true); //****

    // original.multiplyBy(1/original.energy());

    // for (int i=0; i<reconstruction.length(); ++i){
    //   original[i] -= reconstruction[i];
    // }
    // cout << "residual energy: " << original.energy() << endl; //****
  }
  reconstruction.toWav("reconstruction.wav", true);

  cout << "program finished"<< endl;
  return 0;
}
