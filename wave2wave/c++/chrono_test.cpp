// g++ -O3 -std=c++14 -Wall -Wextra /home/afr/git/supercollider-work/wave2wave/c++/MAIN.cpp -o MAIN && valgrind --leak-check=yes ./MAIN

#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;


// 20 million x 10k takes 38 microseconds
const long long LENGTH_S = 20000000; // 20 million samples=7:30
const int NUM_CCMS = 10000; // about 150 samples


void function(){
  long long number = 0;
  for(long long i = 0; i<LENGTH_S; i++){
    for(int j=0; j<NUM_CCMS; j++){
      number +=1;
    }
  }
  cout << "result: " << number<<endl;
}

int main()
{
  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  function();
  high_resolution_clock::time_point t2 = high_resolution_clock::now();

  auto duration = duration_cast<microseconds>( t2 - t1 ).count();

  cout << duration << " microseconds"<< endl;
  return 0;
}
