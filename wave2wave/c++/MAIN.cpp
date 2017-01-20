//g++ -O3 -std=c++14 -Wall -Wextra -pedantic MAIN.cpp floatsignal.cpp wavclient.cpp -o MAIN -lsndfile && valgrind --leak-check=yes --track-origins=yes ./MAIN

//#include <sndfile.hh>

#include <iostream>
// #include <cmath>
// #include <random>
// #include <chrono>
#include "floatsignal.h"


#include "libalglib/fasttransforms.h"

using namespace std;
//using namespace chrono;



//&vec[0]










int main()

{
  string OUT_PATH = "output.wav";
  string ORIGINAL_PATH = "child-short.wav";
  vector<string> MATERIAL_ADDRESSES = {"anvil[0].wav", "anvil[40].wav"};



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



  FloatSignal fs("child-short.wav");
  fs.printSFInfo();


  cout << "main finished"<<endl;
  return 0;
}

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
