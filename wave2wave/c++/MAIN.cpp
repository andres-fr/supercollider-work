// g++ -O3 -std=c++14 -Wall -Wextra -pedantic MAIN.cpp doublesignal.cpp crosscorrelator.cpp -o MAIN -lsndfile -lalglib && valgrind --leak-check=yes --track-origins=yes ./MAIN


//#include <sndfile.hh>

#include <iostream>
#include <vector>
#include <set>
// #include <cmath>
// #include <random>
// #include <chrono>
#include "doublesignal.h"
#include "crosscorrelator.h"

using namespace std;
//using namespace chrono;


// TODO: OK for array rotation. but how to set delays?


int main()

{


  string ORIGINAL_PATH = "child-short.wav";
  vector<string> MATERIAL_ADDRESSES = {"anvil[0].wav", "anvil[40].wav"};


  CrossCorrelator cc(ORIGINAL_PATH, MATERIAL_ADDRESSES);

  // DoubleSignal* getMaterial(const int i);
  // DoubleSignal* getCCoriginal(const int i);
  // DoubleSignal* getCCmaterial(const int i, const int j);

  cc.getMaterial(1)->toWav("mat1.wav",true);
  cc.getCCoriginal(1)->toWav("ccs1.wav",true);
  cc.getCCmaterial(0,1)->toWav("ccm01.wav",true);
  cc.getCCmaterial(1,0)->toWav("ccm10.wav",true);


  // implementa varios metodos de optimizacion:
  //   1) INCREMENTAL: la adjlist esta vacia, y va metiendo.
  //                   para cada metida suma las ccs existentes en la adjist y busca el max
  //   2) OPTIMIZADOR: la adjlist tiene tamaño fijo. Va pillando aleatorios, y
  //                   los optimiza como el num.1






  cout << "test finished"<< endl;
  return 0;
}







  // DoubleSignal cc("output.wav", false);

  // double max = 0;
  // for(int i=0; i<cc.length(); ++i){
  //   if (cc[i] >max){max = cc[i];}
  // }
  // cout << "max: "<< max << endl;

  // double min = 0;
  // for(int i=0; i<cc.length(); ++i){
  //   if (cc[i] <min){min = cc[i];}
  // }
  // cout << "min: "<< min << endl;

  // double acc = 0;
  // for(int i=0; i<cc.length(); ++i){
  //   acc+= cc[i];
  // }
  // cout << "acc: "<< acc << endl;

  // //cc.toWav("output.wav");










  // int LOWER_BOUND = 0;
  // int UPPER_BOUND = 100000;
  // int N = 150;

  // for(int t=LOWER_BOUND; t<UPPER_BOUND; t++){
  //   for(int i=0; i<N; i++){
  //     result[i]= CCS.get(i).at(t);
  //     for(int j=0; j<(N*(N+1)/2);j++){
  //       result[i] -= CCM.get(i,j).at(t);
  //     }
  //   }
  // }// after this loop, result (also a Signal) should

  // double* d = new double[10]();
  // DoubleSignal fs(10, d);
  // delete[] d;




// FloatSignal original(??);
// vector<FloatSignal> materials(??);
// vector<FloatSignal> CCS;
// vector<vector<FloatSignal>> CCM;



// int SIG_SIZE = 10000000;
//   long long LOOP_SIZE = 200000000;
//   high_resolution_clock::time_point t1 = high_resolution_clock::now();
//   FloatSignal fs(SIG_SIZE);
//   for (long long i=0; i<LOOP_SIZE; i++){
//     fs.set(i%SIG_SIZE, i);
//   }
//   high_resolution_clock::time_point t2 = high_resolution_clock::now();

//   auto duration = duration_cast<microseconds>( t2 - t1 ).count();

//   cout << duration << " microseconds"<< endl;
