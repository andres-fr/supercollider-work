// g++ -O3 -std=c++14 -Wall -Wextra -pedantic MAIN.cpp doublesignal.cpp crosscorrelator.cpp freefunctions.cpp -o MAIN -lsndfile -lalglib && valgrind --leak-check=yes ./MAIN

// g++ -O4 -std=c++14 -Wall -Wextra -pedantic MAIN.cpp doublesignal.cpp crosscorrelator.cpp freefunctions.cpp inputparser.cpp -o MAIN -lsndfile -lalglib && ./MAIN -a preprocess -s /home/afr/IMPULS/WORKSPACE/AUDIO/child-short.wav -m /home/afr/IMPULS/WORKSPACE/AUDIO/anvil[0].wav /home/afr/IMPULS/WORKSPACE/AUDIO/anvil[40].wav /home/afr/IMPULS/WORKSPACE/AUDIO/[-40].wav -r 20 -o test_child

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
#include "inputparser.h"

using namespace std;
//using namespace chrono;




// PROFILING:
// develop heuristic to split a file in parts? (sitting in a room f.e.)

// TWEAKING:


// TIDYING UP:
// separate functionality, and enable access through commands
// do a python script, autogenerating man and docs... who does file managing? (f.e. create DIR)

// what do i do now?

//                1) give path to working dir (audio/analysis/output must exist inside)
//                2) give name of orig. and materials in AUDIO
//                3) give sampledown and no. iterations

//                4) calculate all CCS and CCM and save them

//                5) optimization: progressively load CCs and find them a delay and norm
//                6) reconstruction: output a list and wave file with the optimals

// what should be done at the end?
//                the program has two cores: 4 and 5. The most practical approach
//                would be to allow different original+materials for each call!
//                so each call would have as arguments:
//                - path to the orig wave
//                - path to the materials
//                - path of the working dir to be created (deleting prior if existing)
//                - no. of iterations, sampledown rate
//                Which criteria do you think would be appropiate for choosing orig+samples? trying out stuff?
//                    ;
// Developing -- top-down:
// Start with a python3 program that already has this interface (ask tutors if python3 OK. Hanns: maybe java??)
// once interface+documentation is done, adapt c++ parts to respond to it. Minimize the c++ part to what is needed;
// This python program should behave like a server? I dont think so. When the interaction is clear, implement as UGEN! until then, everything AOT. Just keep heavy things on the c++ part AND DO MULTITHREADING IN THE CC PART!!

// -a action
// -s originalName, WHOLE PATH
// -m mat1 mat2 ... WHOLE PATHS
// -r sampledown ratio
// -i iterations
// -o output path
// -d the D-list

// ./MAIN -a process -s <ORIGINAL_PATH> -m <MATERIALS ...> -r 20 -i 400 -o <OUT_PATH>
// ./MAIN -a generate -o <OUT_PATH>


struct metadata {string wavPath; unsigned int size; double energy;};
struct d_ref {metadata meta; int m_id; int del; double k;};

int main(int argc, char **argv){
  const string WORKSPACE_DIR = "/home/afr/IMPULS/WORKSPACE/";
  const string METADATA_NAME = "METADATA.txt";

  InputParser input(argc, argv, WORKSPACE_DIR);  

  // example: ./MAIN
  // -a preprocess -s <ORIGINAL_PATH> -m <MATERIALS ...> -r 20 -o <OUT_PATH>
  if (input.getAction() == "preprocess"){
    string o = input.getOutputPath();
    string cmd1 = "rm -rf "+o;
    string cmd2 = "mkdir "+o;
    string cmd3 = "mkdir "+o+"ANALYSIS";
    string cmd4 = "mkdir "+o+"OUTPUT";
    int response1 = system(cmd1.c_str());
    if (response1!=0){
      cout << "aborting: couldn't make " << o << endl;
      return 1;
    } else{ // if folder could be created
      int response2 = system(cmd2.c_str());
      int response3 = system(cmd3.c_str());
      int response4 = system(cmd4.c_str());
      if (response2+response3+response4!=0){
        cout << "aborting: couldn't make subfolders in " << o << endl;
        return 1;
      } else {// if subfolders could be created
        // make METADATA
        CrossCorrelator cc(WORKSPACE_DIR, o);
        cc.loadAudios(input.getOriginalPath(), input.getMaterialPaths(), METADATA_NAME);
        cc.calculateCCs(input.getSampledownRatio());
        // calculate and store CCs in o+ANALYSIS
      }
    }
  }







  
  // example: ./MAIN
  // -a generate_list -i 1000 -o <OUTPATH>
  if (input.getAction() == "generate_list"){
    // 1) read metadata in OUTPATH
    // 2) optimize and output the list
  }

  // example: ./MAIN
  // -a generate_list -o <OUTPATH>
  if (input.getAction() == ""){
    cout << "guadefac"<< endl;
  }
  
  
  return 0;

  // cout << "version changeds..." << endl;

  // string WORKING_DIR = "/home/afr/git/supercollider-work/wave2wave/SETUP_CHILD_SAMPLEDOWN_16/";
  // string AUDIO_DIR = WORKING_DIR+"AUDIO/";
  // string ANALYSIS_DIR = WORKING_DIR+"ANALYSIS/";
  // string OUTPUT_DIR = WORKING_DIR+"OUTPUT/";
  // string METADATA_ADDRESS = WORKING_DIR+"METADATA.txt";
  // string ORIGINAL_NAME = "child-short.wav";
  // string ORIGINAL_PATH = AUDIO_DIR+ORIGINAL_NAME;
  // string WAV2WAV_PATH = OUTPUT_DIR+"wav2wav.txt";
  // string WAVOUT_PATH = OUTPUT_DIR+"reconstruction.wav";
  // vector<string> MATERIAL_PATHS;//{"test_m1.wav", "test_m2.wav", "test_orig.wav", "31line.wav"};

  // unsigned int SAMPLEDOWN_RATIO {1};
  // cout << "type sampledown ratio: ";
  // cin >> SAMPLEDOWN_RATIO;

  // cout << "type 'yes' if you wish to calculate all cross-correlations "
  //      << "(needed if sampledown ratio has changed): ";
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

  // // declare and load METADATA: idx 0 is original, rest are materials
  // unsigned int MAX_LEN = 0; // the size in samples of the biggest CC file
  // double MAX_ENERGY = 0;
  // vector<metadata> METADATA; // (wavPath, size, energy)
  // ifstream inSpecs(METADATA_ADDRESS.c_str());
  // if(inSpecs.is_open()==false){
  //   cout << "couldnt open " << METADATA_ADDRESS << endl;
  // } else{
  //   string s;
  //   while(inSpecs >>s){
  //     int sepPos = s.find_first_of(':');
  //     string wavpath = s.substr(0, sepPos);
  //     string rest = s.substr(sepPos+1);
  //     sepPos = rest.find_first_of(':');
  //     unsigned int len = stoi(rest.substr(0, sepPos));
  //     double energy = stod(rest.substr(sepPos+1));
  //     METADATA.push_back(metadata{wavpath, len, energy});
  //     MAX_LEN = (len>MAX_LEN)? len : MAX_LEN;
  //     MAX_ENERGY = (energy>MAX_ENERGY)? energy : MAX_ENERGY;
  //   }
  // }

  // // some variables outside of the for-loop
  // unsigned int ORIG_LEN = METADATA[0].size;
  // // the wav2wav vector to be filled
  // vector<d_ref> D; // d_ref {metadata meta; int m_id; int del; double k;}
  // // the reconstruction placeholder
  // DoubleSignal reconstruction(ORIG_LEN);
  // reconstruction.setSFInfo(reconstruction.length(), 44100, 1, 65538, 1, 1);


  // // prepare random generation:
  // // a call to distribution(gen) should generate unif. ints between the second
  // // and last idx of metadata (corresponding to the materials)
  // // WARNING: idx 1 corresponds to m0!
  // random_device rd;
  // mt19937 gen(static_cast<mt19937::result_type>(time(nullptr)));
  // uniform_int_distribution<> distribution(1, METADATA.size()-1);

  // MAX_LEN = downsamplingLength(MAX_LEN, SAMPLEDOWN_RATIO);
  // unsigned int TEMPSIG_LEN = 2*MAX_LEN; // so now we know zero is at tempSig[MAX_LEN]


  // //////////////////////////////////////////////////////////////////////////////
  // /// LOOP
  // //////////////////////////////////////////////////////////////////////////////
  // int counter = 0;
  // for (int i=0; i<LOOP_SIZE; ++i){
  //   cout << "processing " << counter << "/" << LOOP_SIZE << endl;
  //   counter++;
  // //for (int r : {1,2,0,0,1}){

  //   const int r = distribution(gen)-1;
  //   const unsigned int m_size = METADATA[r+1].size;
  //   const unsigned int mshift_len = (m_size-1)/SAMPLEDOWN_RATIO;
  //   // CALCULATE DOWNSAMPLED LENGTH
  //   if(m_size>ORIG_LEN){
  //     cout <<"ignoring "<< METADATA[r+1].wavPath <<": material longer than original!"<< endl;
  //   } else { // if chosen material isn't longer than original...
  //     // initialize local containers for iteration
  //     DoubleSignal* tempSig = new DoubleSignal(TEMPSIG_LEN); // zeros at beg.

  //     double maxVal = 0;//- numeric_limits<double>::infinity(); // initialize at -inf
  //     int maxPos = 0;


  //     // for each material in D...
  //     for(d_ref &d : D){ // wavPath, m_id, del, k
  //       // get the name of the corresponding CCMs
  //       pair<int, int> tup;
  //       if (d.m_id<r){
  //         tup.first = d.m_id;
  //         tup.second = r;
  //       } else {
  //         tup.first = r;
  //         tup.second = d.m_id;
  //       }
  //       string ccName = "cc_m"+to_string(tup.first)+"_m"+to_string(tup.second)+".wav";
  //       DoubleSignal ccm(ANALYSIS_DIR+ccName, false);
  //       if(r<d.m_id){ccm.reverse();} // ensure that our r is always the "shifting" sig
  //       for(int i=0; i<ccm.length(); ++i){
  //         tempSig->decrementAt(i+MAX_LEN+d.del-mshift_len, ccm[i]*d.k);
  //       }
  //       //tempSig->prettyPrint("tempSig after substracting "+ccName);
  //     }

  //     // now get the CCS and add it to tempSig
  //     string ccs_name = ANALYSIS_DIR+"cc_original_m"+to_string(r)+".wav";
  //     DoubleSignal ccs(ccs_name, false);
  //     for(int i=0; i<tempSig->length(); ++i){
  //       (*tempSig)[i] += ccs.at((i-MAX_LEN)+mshift_len, 0);
  //       if(abs((*tempSig)[i])>abs(maxVal)){
  //         maxVal = (*tempSig)[i];
  //         maxPos = i-MAX_LEN;
  //       }
  //     }
  //     //tempSig->prettyPrint("tempSig after adding ccs");


  //     // find maximum in tempSig, and add result to D
  //     double k_factor = maxVal * MAX_ENERGY / (METADATA[r+1].energy); //******************
  //     D.push_back(d_ref{METADATA[r+1], r, maxPos,k_factor});// maxPos, k_factor});
  //     // cout << i+1 << "/" << LOOP_SIZE << ") "<< "added signal " << r <<
  //     //   " with delay "  << maxPos << " and norm " << k_factor << endl << endl;
  //     // delete instantiated signals before finishing iteration
  //     delete tempSig;

  //   }
  // }



  // //////////////////////////////////////////////////////////////////////////////
  // /// END LOOP
  // //////////////////////////////////////////////////////////////////////////////


  // // SAVE D TO TXT FILE
  // ofstream outFile(WAV2WAV_PATH, ios::out);
  // if(outFile.is_open()){
  //   cout << "writing " << WAV2WAV_PATH << endl;
  //   for(d_ref d : D){
  //     outFile << "material:" << d.meta.wavPath << " delay:" << d.del*SAMPLEDOWN_RATIO <<
  //       " normFactor:" << d.k << endl;
  //   }
  //   outFile.close();
  //   cout << "wrote "<< WAV2WAV_PATH << " succesfully!" << endl;
  // }





  // // RECONSTRUCT SIGNAL (inefficient: better do it with inverse CCs!!)
  // counter = 0;
  // double last_residual = numeric_limits<double>::infinity();
  // double current_residual = 0;
  // for (d_ref d : D){ //d_ref meta, m_id, del, k // metadata: wavPath, size, energy
  //   counter++;
  //   DoubleSignal ds(d.meta.wavPath, true); // ds is a material, NORMALIZED
  //   for (int i=0; i<reconstruction.length(); ++i){
  //     reconstruction[i] += (ds.at(i, d.del*SAMPLEDOWN_RATIO))*d.k;
  //   }

  //   DoubleSignal original(ORIGINAL_PATH,true); // original, NORMALIZED
  //   for (int i=0; i<reconstruction.length(); ++i){
  //     original[i] -= reconstruction[i];
  //   }
  //   cout << counter << "/" << LOOP_SIZE << ") "<< "added signal " << d.m_id <<
  //     " with delay "  << d.del*SAMPLEDOWN_RATIO << " and norm " << d.k << endl;
  //   current_residual = original.energy();
  //   cout << "residual energy: " << current_residual << endl;
  //   if (current_residual> last_residual){
  //     cout << "WARNING: residual energy raised: " <<
  //       last_residual << "===>" << current_residual << endl <<
  //       "this should be a very small increment, probably involving noise or " <<
  //       "affecting samples that are partly positioned out of the borders.\n\n";
  //   }
  //   last_residual = current_residual;
  // }

  // reconstruction.toWav(WAVOUT_PATH, true);



  // cout << "program finished"<< endl;
  // return 0;


  
}




  // DoubleSignal a(AUDIO_DIR+"flexatone.wav", true);
  // DoubleSignal b(80000);
  // for(int i=0; i<80000; ++i){
  //   b[i] = a[i];
  // }
  // b.setSFInfo(80000, 44100, 1, 65538, 1, 1);
  // b.toWav("flexatone-short.wav", true);
  // cout << "done!!"<< endl;
