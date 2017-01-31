// stl includes
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <string>
#include <algorithm>
// other libraries

// own dependencies

// own header
#include "freefunctions.h"
// namespaces
using namespace std;


void normalizeArray(double* contents, const int sz, const double mean,
                    const double maxVal){
  cout << "normalizing"<< endl;
  int i=0;
  double sum = 0;
  double max = 0;
  // calculate mean into accum and get absolute maximum
  for(i=0; i<sz; i++){
    sum += contents[i];
    if (abs(contents[i]) > abs(max)){
      max = contents[i];
    }
  }
  double deltamean = (sum/sz)-mean;
  double deltanorm = maxVal / (max-deltamean);
  // substract accum to each sample, and renormalize
  for(i=0; i<sz; i++){
    contents[i] = (contents[i]-deltamean)*deltanorm;
  }
}


void rotateArray(double* contents, const int size, const int pivot){
  cout << "rotating array..." << endl;
  reverse(contents, contents+size);
  reverse(contents, contents+size-pivot);
  reverse(contents+size-pivot, contents+size);
}



