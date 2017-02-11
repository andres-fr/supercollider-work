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
// sample down analysis to 1:3, and check it still works on child-short
// if still working, try with 1:4
//


struct metadata {string wavPath; unsigned int size; double energy;};
struct d_ref {metadata meta; int m_id; int del; double k;};

int main(){

  cout << "version changed ..." << endl;

  string WORKING_DIR = "/home/afr/git/supercollider-work/wave2wave/SETUP_CHILD_SAMPLEDOWN_16/";
  string AUDIO_DIR = WORKING_DIR+"AUDIO/";
  string ANALYSIS_DIR = WORKING_DIR+"ANALYSIS/";
  string OUTPUT_DIR = WORKING_DIR+"OUTPUT/";
  string METADATA_ADDRESS = WORKING_DIR+"METADATA.txt";
  string ORIGINAL_NAME = "child-short.wav";
  string ORIGINAL_PATH = AUDIO_DIR+ORIGINAL_NAME;
  string WAV2WAV_PATH = OUTPUT_DIR+"wav2wav.txt";
  string WAVOUT_PATH = OUTPUT_DIR+"reconstruction.wav";
  vector<string> MATERIAL_PATHS;//{"test_m1.wav", "test_m2.wav", "test_orig.wav", "31line.wav"};
  // unsigned int SAMPLEDOWN_RATIO = 2;

  // cout << "type 'yes' if you wish to calculate all cross-correlations: ";
  // string yn;
  // cin >> yn;
  // if (yn=="yes"){
  //   for(int i=-100; i<=100; i+=4){
  //     MATERIAL_PATHS.push_back("anvil["+to_string(i)+"].wav");
  //   }
  //   CrossCorrelator cc(ORIGINAL_NAME,MATERIAL_PATHS,WORKING_DIR,SAMPLEDOWN_RATIO);
  // }
  // int LOOP_SIZE = 0;
  // cout << "type number of iterations: ";
  // cin >> LOOP_SIZE;

  unsigned int SAMPLEDOWN_RATIO;
  cout << "type int for sampledown ratio: ";
  cin >> SAMPLEDOWN_RATIO;
  int LOOP_SIZE = 200;
  for(int i=-100; i<=100; i+=4){
    MATERIAL_PATHS.push_back("anvil["+to_string(i)+"].wav");
  }
  CrossCorrelator cc(ORIGINAL_NAME,MATERIAL_PATHS,WORKING_DIR,SAMPLEDOWN_RATIO);

  // declare and load METADATA: idx 0 is original, rest are materials
  unsigned int MAX_LEN = 0; // the size in samples of the biggest CC file
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
      unsigned int len = stoi(rest.substr(0, sepPos));
      double energy = stod(rest.substr(sepPos+1));
      METADATA.push_back(metadata{wavpath, len, energy});
      MAX_LEN = (len>MAX_LEN)? len : MAX_LEN;
      MAX_ENERGY = (energy>MAX_ENERGY)? energy : MAX_ENERGY;
    }
  }

  // some variables outside of the for-loop
  unsigned int ORIG_LEN = METADATA[0].size;
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



  // readapt MAX_LEN to the current downsampling settings
  unsigned int ORIG_LEN_DOWNSAMPLED = downsamplingLength(ORIG_LEN, SAMPLEDOWN_RATIO);
  //unsigned int TEMPSIG_LEN = downsamplingLength(2*MAX_LEN-1, SAMPLEDOWN_RATIO); //********
  MAX_LEN = downsamplingLength(MAX_LEN, SAMPLEDOWN_RATIO);
  unsigned int TEMPSIG_LEN = 2*MAX_LEN; // so now we know zero is at tempSig[MAX_LEN]


  //////////////////////////////////////////////////////////////////////////////
  /// LOOP
  //////////////////////////////////////////////////////////////////////////////
  int counter = 0;
  for (int i=0; i<LOOP_SIZE; ++i){
    cout << "processing " << counter << "/" << LOOP_SIZE << endl;
    counter++;
  //for (int r : {1,2,0,0,1}){

    int r = distribution(gen)-1;
    // CALCULATE DOWNSAMPLED LENGTH
    if(METADATA[r+1].size>ORIG_LEN){
      cout <<"ignoring "<< METADATA[r+1].wavPath <<": material longer than original!"<< endl;
    } else { // if chosen material isn't longer than original...
      // initialize local containers for iteration
      DoubleSignal* tempSig = new DoubleSignal(TEMPSIG_LEN); // zeros at beg.

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
        DoubleSignal ccm(ANALYSIS_DIR+ccName, false);
        if(r<d.m_id){ccm.reverse();} // ensure that our r is always the "shifting" sig
        // const unsigned int mshift_len = ccm.length()-downsamplingLength(d.meta.size, SAMPLEDOWN_RATIO);
        const unsigned int mshift_len = (METADATA[r+1].size-1)/SAMPLEDOWN_RATIO;
        for(int i=0; i<ccm.length(); ++i){
          tempSig->decrementAt(i+MAX_LEN+d.del-mshift_len, ccm[i]*d.k);
        }
        //tempSig->prettyPrint("tempSig after substracting "+ccName);
      }


      // now get the CCS and add it to tempSig
      string ccs_name = ANALYSIS_DIR+"cc_original_m"+to_string(r)+".wav";
      DoubleSignal ccs(ccs_name, false);
      // const unsigned int mshift_len = ccs.length()-ORIG_LEN_DOWNSAMPLED;
      const unsigned int mshift_len = (METADATA[r+1].size-1)/SAMPLEDOWN_RATIO;
      for(int i=0; i<tempSig->length(); ++i){ //********
        (*tempSig)[i] += ccs.at((i-MAX_LEN)+mshift_len, 0);
        if(abs((*tempSig)[i])>abs(maxVal)){
          maxVal = (*tempSig)[i];
          maxPos = i-MAX_LEN;
        }
      }
      //tempSig->prettyPrint("tempSig after adding ccs");


      // // now get the CCS and add it to tempSig
      // string ccs_name = ANALYSIS_DIR+"cc_original_m"+to_string(r)+".wav";
      // DoubleSignal ccs(ccs_name, false);
      // for(int i=0; i<tempSig->length(); ++i){ //********
      //   (*tempSig)[i] += ccs.at((i-MAX_LEN)+m_length, 0);
      //   if(abs((*tempSig)[i])>abs(maxVal)){
      //     maxVal = (*tempSig)[i];
      //     maxPos = i-(MAX_LEN-1);
      //   }
      // }



      // find maximum in tempSig, and add result to D
      double k_factor = maxVal * MAX_ENERGY / (METADATA[r+1].energy); //******************
      D.push_back(d_ref{METADATA[r+1], r, maxPos,k_factor});// maxPos, k_factor});
      // cout << i+1 << "/" << LOOP_SIZE << ") "<< "added signal " << r <<
      //   " with delay "  << maxPos << " and norm " << k_factor << endl << endl;
      // delete instantiated signals before finishing iteration
      delete tempSig;

    }
  }



  //////////////////////////////////////////////////////////////////////////////
  /// END LOOP
  //////////////////////////////////////////////////////////////////////////////


  // SAVE D TO TXT FILE
  ofstream outFile(WAV2WAV_PATH, ios::out);
  if(outFile.is_open()){
    cout << "writing " << WAV2WAV_PATH << endl;
    for(d_ref d : D){
      outFile << "material:" << d.meta.wavPath << " delay:" << d.del*SAMPLEDOWN_RATIO <<
        " normFactor:" << d.k << endl;
    }
    outFile.close();
    cout << "wrote "<< WAV2WAV_PATH << " succesfully!" << endl;
  }





  // RECONSTRUCT SIGNAL (inefficient: better do it with inverse CCs!!)
  counter = 0;
  double last_residual = numeric_limits<double>::infinity();
  double current_residual = 0;
  for (d_ref d : D){ //d_ref meta, m_id, del, k // metadata: wavPath, size, energy
    counter++;
    DoubleSignal ds(d.meta.wavPath, true); // ds is a material, NORMALIZED
    for (int i=0; i<reconstruction.length(); ++i){
      reconstruction[i] += (ds.at(i, d.del*SAMPLEDOWN_RATIO))*d.k;
    }

    DoubleSignal original(ORIGINAL_PATH,true); // original, NORMALIZED
    for (int i=0; i<reconstruction.length(); ++i){
      original[i] -= reconstruction[i];
    }
    cout << counter << "/" << LOOP_SIZE << ") "<< "added signal " << d.m_id <<
      " with delay "  << d.del*SAMPLEDOWN_RATIO << " and norm " << d.k << endl;
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

  reconstruction.toWav(WAVOUT_PATH, true);



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
