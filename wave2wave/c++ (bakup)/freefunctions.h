#ifndef FREEFUNCTIONS_H
#define FREEFUNCTIONS_H

// stl includes
#include <string>
// other libraries

// own dependencies

// namespaces
using namespace std;


/* void normalizeArray(double* contents, const int sz, const double mean, */
/*                     const double maxVal); */

void normalizeArray(double* contents, const int sz);

void rotateArray(double* contents, const int size, const int idxOfLowest);

void array2txt(double* contents, const int size, const string pathOut);

unsigned int downsamplingLength(const unsigned int origLength,
                                const unsigned int downSampleRatio);

#endif
