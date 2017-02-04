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


struct metadata {string name; int size; double energy;};
struct d_ref {string name; int id; int del; double k;};

int main(){

  string AUDIO_DIR = "/home/afr/git/supercollider-work/wave2wave/AUDIO/";
  string ANALYSIS_DIR = "/home/afr/git/supercollider-work/wave2wave/ANALYSIS/";
  string OUTPUT_DIR = "/home/afr/git/supercollider-work/wave2wave/OUTPUT/";
  string METADATA_ADDRESS = ANALYSIS_DIR+"METADATA.txt";


  int LOOP_SIZE = 20;





  // vector<string> MATERIAL_ADDRESSES;
  // for (int i=-500; i<=500; i+=4){
  //   MATERIAL_ADDRESSES.push_back(AUDIO_DIR+"anvil["+to_string(i)+"].wav");
  // }
  // CrossCorrelator cc(AUDIO_DIR+"child-short.wav", MATERIAL_ADDRESSES, ANALYSIS_DIR);

  // initialize adj.list





  // declare and load spec vectors: [0] original, rest are materials
  int MAX_LEN = 0;
  vector<metadata> METADATA;
  ifstream inSpecs(METADATA_ADDRESS.c_str());
  if(inSpecs.is_open()==false){
    cout << "couldnt open " << METADATA_ADDRESS << endl;
  } else{
    string s;
    while(inSpecs >>s){
      int sepPos = s.find_first_of(':');
      string name = s.substr(0, sepPos);
      string rest = s.substr(sepPos+1);
      sepPos = rest.find_first_of(':');
      int len = stoi(rest.substr(0, sepPos));
      double energy = stod(rest.substr(sepPos+1));
      METADATA.push_back(metadata{name, len, energy});
      MAX_LEN = (len>MAX_LEN)? len : MAX_LEN;
    }
  }


  // prepare random generation
  random_device rd;
  mt19937 gen(static_cast<mt19937::result_type>(time(nullptr)));
  uniform_int_distribution<> distribution(1, METADATA.size()-1);


  // prepare containers for optimization algorithm:
  vector<d_ref>D; // {name, id, delay, k} //  grows+1 per each iter
  map<string, DoubleSignal*>temp; //

  for (int i=0; i<LOOP_SIZE; i++){
    // pick a random index
    int r = distribution(gen)-1;
    // // get corresponding CCS
    // metadata* md = &METADATA[r];
    // edit md->name to get the CCS
    string ccs = ANALYSIS_DIR+"cc_original_m"+to_string(r)+".txt";
    // if signal not in map, load and add!
    if (temp.find(ccs)==temp.end()){
      temp.insert(make_pair(ccs, new DoubleSignal(ccs)));
    } // now temp[ccs] should work

    // load the corresponding CC[mi,mj] for each mj in the D list:
    for (d_ref d : D){
      string subname = (d.id<r)? "m"+to_string(d.id)+"_m"+to_string(r)
        : "m"+to_string(r)+"_m"+to_string(d.id);
      string ccm = ANALYSIS_DIR+"cc_"+subname+".txt"; // cc_mi_mj.txt
      // if signal not in map, load and add!
      if (temp.find(ccm)==temp.end()){
        temp.insert(make_pair(ccm, new DoubleSignal(ccm)));
      }
    }
    // at this point we should have all doubleSignals there.
    // it simply remains to add the CCS and substract the existing
    // CCMs REGARDING EACH DELAY AND NORMALIZATION! possible approach:
    double maxVal = - numeric_limits<double>::infinity();
    int maxPos = 0;

    DoubleSignal* tempSig = new DoubleSignal(2*MAX_LEN-1);
    // substract ccms from tempsig
    for (d_ref d : D){
      string subname = (d.id<r)? "m"+to_string(d.id)+"_m"+to_string(r)
        : "m"+to_string(r)+"_m"+to_string(d.id);
      string ccm = ANALYSIS_DIR+"cc_"+subname+".txt"; // cc_mi_mj.txt
      for (int i=0; i<(2*MAX_LEN-1); ++i){
        (*tempSig)[i] -= (temp[ccm]->at(i, d.del)) * d.k;
      }
    }
    // add ccs to tempsig
    for (int i=0; i<(2*MAX_LEN-1); ++i){
      (*tempSig)[i] += (temp[ccs]->at(i, MAX_LEN-1));
      // and get max and pos
      if ((*tempSig)[i]>maxVal){
        maxVal = (*tempSig)[i];
        maxPos = i;
      }
    }
    // // after this loop it should be clear that the choosen sample
    // // goes to position maxPos, and its optimal k=max/energy(m)
    // // this should be appended to D
    D.push_back(d_ref{METADATA[r+1].name, r, maxPos, maxVal/(METADATA[r+1].energy)});
    cout << "added signal " << r << " with delay "  << maxPos << " and norm " << maxVal/(METADATA[r+1].energy) << endl;
    delete tempSig;
  }



  // RECONSTRUCT SIGNAL (inefficient: better do it with inverse CCs!!)
  DoubleSignal* reconstruction = new DoubleSignal(2*MAX_LEN-1);
  reconstruction->setSFInfo(2*MAX_LEN-1, 44100, 1, 65538, 1, 1);
  // substract ccms from tempsig
  for (d_ref d : D){
    DoubleSignal ds(d.name, true);
    for (int i=0; i<(2*MAX_LEN-1); ++i){
      (*reconstruction)[i] += (ds.at(i, d.del)) * d.k;
    }
  }
  reconstruction->toNormalizedWav("reconstruction.wav");
  delete reconstruction;



  // delete contents of temp before finishing
  for(auto &x : temp){
    delete x.second;
  }

  cout << "test finished"<< endl;
  return 0;
}
