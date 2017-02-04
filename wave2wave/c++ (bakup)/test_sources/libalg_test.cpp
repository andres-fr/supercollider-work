// g++ -std=c++14 -Wall -Wextra /home/afr/git/supercollider-work/wave2wave/c++/libalg_test.cpp -o libalg_test -lalglib && ./libalg_test
// valgrind --leak-check=yes ./libalg_test

#include <iostream>
#include <random>
#include "libalglib/fasttransforms.h"
using namespace std;


int main(){

  // iter variable
  int i;

  // generate white noise signal with distribution(gen)
  random_device rd;
  mt19937 gen(static_cast<mt19937::result_type>(time(nullptr)));
  uniform_real_distribution<double> distribution(-1.0,1.0);

  // declare signal containers
  double noisearr[20];
  double noise_delayed_arr[30];

  // fill containers
  for (i=0; i<20; i++){
    noisearr[i] = distribution(gen);
  }
  for (i=0; i<30; i++){
    noise_delayed_arr[i] = (i<10)? 0 : noisearr[i-10];
  }

  // wrap containers
  alglib::real_1d_array noise;
  noise.setcontent(20, noisearr);
  alglib::real_1d_array noise_del;
  noise_del.setcontent(30, noise_delayed_arr);
  alglib::real_1d_array result;

  // CALCULATE CROSS-CORRELATION:
  // at result[0] dotprod of both beginning same time
  // until 29, the noise is gradually delayed, 29 is the "last"
  // the entries 30 to 48 correspond to delays -19 to -1
  alglib::corrr1d(noise_del, 30, noise, 20, result);

  // print result and return program
  for(i=0; i<48; i++){cout<<result(i)<<endl;}
  return 0;
}
