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
  cc.getCCoriginals()->at(0)->toWav("out0.wav",true);
  cc.getCCoriginals()->at(1)->toWav("out1.wav",true);



  // DoubleSignal ch("child-short.wav", true);
  // DoubleSignal anv("anvil.wav", true);
  // DoubleSignal result;
  // alglib::corrr1d(ch, ch.length(), anv, anv.length(), result);

  // SF_INFO* sfi = ch.getSFInfo();
  // int sa,c,f,sec,seek;
  // sa = sfi->samplerate;
  // c = sfi->channels;
  // f = sfi->format;
  // sec = sfi->sections;
  // seek = sfi->seekable;

  // result.setSFInfo(result.length(), sa, c, f, sec, seek);

  // result.printSFInfo();
  // cout << endl << result.length() << endl <<endl;

  // result.toWav("output.wav", true);




  // DoubleSignal noise;
  // noise.setcontent(20, new double[20]{0.815803, -0.538149, -0.299479, 0.498359,
  //       0.969564, -0.667747, 0.793966, -0.957624, 0.826445, -0.255609, 0.347082,
  //       0.080027, -0.403685, -0.058940, -0.257758, -0.569333, 0.646189,
  //       0.084176, -0.648250, 0.709720});
  // DoubleSignal noise_del;
  // noise_del.setcontent(30, new double[30]());
  // for (int i=0; i<20; ++i){
  //   noise_del[i+10] = noise[i];
  // }
  // DoubleSignal result;

  // // CALCULATE CROSS-CORRELATION:
  // // at result[0] dotprod of both beginning same time
  // // until 29, the noise is gradually delayed, 29 is the "last"
  // // the entries 30 to 48 correspond to delays -19 to -1
  // alglib::corrr1d(noise_del, 30, noise, 20, result);

  // // print result and return program
  // for(int i=0; i<48; i++){cout<<result(i)<<endl;}





  // AHORA TENEMOS UN DoubleSignal MAS O MENOS ESTABLE.
  // el rollo es calcular las cc, y que sean comparables entre ellas
  // para ello tienes q aclararte como ostias funciona el autonorm en I/O.

  // suponiendo q ya podamos trabajar con CCs tranquilamente,
  // el siguiente paso es implementar la clase OPTIMIZER:

  // el optimizer recibe un nombre de original, y un vector de strings de materials.
  // tiene una adj.list m_id--> delays
  // carga las señales en maps
  // carga las CCS y CCM en maps
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
