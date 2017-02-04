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


// problem/bug: if material is longer than original, dividing by mat.energy will
// make k TOO SMALL, because the m.samples that lie outside the range (as well
// as their energy) are disregarded. Possible fix: divide by the energy of the
// subsignal that lies within the valid range only.

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

  string WORKING_DIR = "/home/afr/git/supercollider-work/wave2wave/SETUP_FLEXATONE/";
  string AUDIO_DIR = WORKING_DIR+"AUDIO/";
  string ANALYSIS_DIR = WORKING_DIR+"ANALYSIS/";
  string OUTPUT_DIR = WORKING_DIR+"OUTPUT/";
  string METADATA_ADDRESS = WORKING_DIR+"METADATA.txt";
  string ORIGINAL_NAME = "flexatone-short.wav";// "child-short.wav";
  string ORIGINAL_PATH = AUDIO_DIR+ORIGINAL_NAME;
  vector<string> MATERIAL_PATHS;//{"test_m1.wav", "test_m2.wav", "test_orig.wav", "31line.wav"};

  // for(int i=-500; i<=500; i+=4){
  //   MATERIAL_PATHS.push_back("anvil["+to_string(i)+"].wav");
  // }
  // CrossCorrelator cc(ORIGINAL_NAME, MATERIAL_PATHS, WORKING_DIR);




  int LOOP_SIZE = 0;
  cout << "type number of iterations: ";
  cin >> LOOP_SIZE;



  // declare and load METADATA: idx 0 is original, rest are materials
  int MAX_LEN = 0; // the size in samples of the biggest CC file
  double MAX_ENERGY = 0;
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
      MAX_ENERGY = (energy>MAX_ENERGY)? energy : MAX_ENERGY;
    }
  }

  // some variables outside of the for-loop
  int ORIG_LEN = METADATA[0].size;
  // the wav2wav vector to be filled
  vector<d_ref> D; // d_ref {metadata meta; int m_id; int del; double k;}
  // the reconstruction placeholder
  DoubleSignal reconstruction(ORIG_LEN);
  reconstruction.setSFInfo(reconstruction.length(), 44100, 1, 65538, 1, 1);


  // prepare random generation:
  // a call to distribution(gen) should generate unif. ints between the second
  // and last idx of metadata (corresponding to the materials)
  // WARNING: idx 1 corresponds to m0!
  random_device rd;
  mt19937 gen(static_cast<mt19937::result_type>(time(nullptr)));
  uniform_int_distribution<> distribution(1, METADATA.size()-1);





  //////////////////////////////////////////////////////////////////////////////
  /// LOOP
  //////////////////////////////////////////////////////////////////////////////
  int counter = 0;
  for (int i=0; i<LOOP_SIZE; ++i){
    cout << "processing " << counter << "/" << LOOP_SIZE << endl;
    counter++;
  //for (int r : {1,2,0,0,1}){

    int r = distribution(gen)-1;
    // load ccs
    string ccs_name = ANALYSIS_DIR+"cc_original_m"+to_string(r)+".wav";
    DoubleSignal ccs(ccs_name, false);
    if(ccs.length()> 2*ORIG_LEN-1){
      cout <<"ignoring "<< ccs_name <<": material longer than original!"<< endl;
    } else { // if chosen material isn't longer than original...

      // initialize local containers for iteration
      DoubleSignal* tempSig = new DoubleSignal(2*MAX_LEN-1); // zeros at beg.

      double maxVal = 0;//- numeric_limits<double>::infinity(); // initialize at -inf
      int maxPos = 0;

      // for each material in D...
      for(d_ref &d : D){ // wavPath, m_id, del, k
        // get the name of the corresponding CCMs
        pair<int, int> tup;
        if (d.m_id<r){
          tup.first = d.m_id;
          tup.second = r;
        } else {
          tup.first = r;
          tup.second = d.m_id;
        }
        string ccName = "cc_m"+to_string(tup.first)+"_m"+to_string(tup.second)+".wav";
        DoubleSignal m(ANALYSIS_DIR+ccName, false);
        if(r<d.m_id){m.reverse();} // ensure that our r is always the "shifting" sig
        for(int i=0; i<m.length(); ++i){
          tempSig->decrementAt(i+MAX_LEN+d.del-METADATA[r+1].size, m[i]*d.k);
        }
        //tempSig->prettyPrint("tempSig after substracting "+ccName);
      }

      // now get the CCS and add it to tempSig
      for(int i=0; i<tempSig->length(); ++i){ //********
        (*tempSig)[i] += ccs.at((i-MAX_LEN)+METADATA[r+1].size, 0);
        if(abs((*tempSig)[i])>abs(maxVal)){
          maxVal = (*tempSig)[i];
          maxPos = i-(MAX_LEN-1);
        }
      }
      // find maximum in tempSig, and add result to D
      double k_factor = maxVal * MAX_ENERGY / (METADATA[r+1].energy); //******************
      D.push_back(d_ref{METADATA[r+1], r, maxPos, k_factor});
      // cout << i+1 << "/" << LOOP_SIZE << ") "<< "added signal " << r <<
      //   " with delay "  << maxPos << " and norm " << k_factor << endl << endl;
      // delete instantiated signals before finishing iteration
      delete tempSig;

    }
  }



  //////////////////////////////////////////////////////////////////////////////
  /// END LOOP
  //////////////////////////////////////////////////////////////////////////////







  // RECONSTRUCT SIGNAL (inefficient: better do it with inverse CCs!!)
  counter = 0;
  double last_residual = numeric_limits<double>::infinity();
  double current_residual = 0;
  for (d_ref d : D){ //d_ref meta, m_id, del, k // metadata: wPath, size, energy
    counter++;
    DoubleSignal ds(d.meta.wavPath, true); // ds is a material, NORMALIZED
    for (int i=0; i<reconstruction.length(); ++i){
      reconstruction[i] += (ds.at(i, d.del))*d.k;
    }

    DoubleSignal original(ORIGINAL_PATH,true); // original, NORMALIZED
    for (int i=0; i<reconstruction.length(); ++i){
      original[i] -= reconstruction[i];
    }
    cout << counter << "/" << LOOP_SIZE << ") "<< "added signal " << d.m_id <<
      " with delay "  << d.del << " and norm " << d.k << endl;
    current_residual = original.energy();
    cout << "residual energy: " << current_residual << endl;
    if (current_residual> last_residual){
      cout << "WARNING: residual energy raised: " <<
        last_residual << "===>" << current_residual << endl <<
        "this should be a very small increment, probably involving noise or " <<
        "affecting samples that are partly positioned out of the borders.\n\n";
    }
    last_residual = current_residual;
  }

  reconstruction.toWav(OUTPUT_DIR+"reconstruction.wav", true);

  //normalize reconstruction:
  normalizeArray(reconstruction.getcontent(), reconstruction.length());
  //reconstruction.prettyPrint("reconstruction end");









  cout << "program finished"<< endl;
  return 0;

}




  // DoubleSignal a(AUDIO_DIR+"flexatone.wav", true);
  // DoubleSignal b(80000);
  // for(int i=0; i<80000; ++i){
  //   b[i] = a[i];
  // }
  // b.setSFInfo(80000, 44100, 1, 65538, 1, 1);
  // b.toWav("flexatone-short.wav", true);
  // cout << "done!!"<< endl;
