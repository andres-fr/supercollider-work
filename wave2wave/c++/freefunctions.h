#ifndef FREEFUNCTIONS_H
#define FREEFUNCTIONS_H

// stl includes
#include <string>
// other libraries

// own dependencies

// namespaces
using namespace std;


void normalizeArray(double* contents, const int sz, const double mean,
                    const double maxVal);


void rotateArray(double* contents, const int size, const int idxOfLowest);


int hashTriangle(const int i, const int j, const int N);

#endif