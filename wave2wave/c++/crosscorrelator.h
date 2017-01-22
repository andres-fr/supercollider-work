#ifndef CROSSCORRELATOR_H
#define CROSSCORRELATOR_H

// stl includes
#include <vector>
#include <map>
// other libraries
#include "sndfile.hh"
#include "doublesignal.h"
#include "libalglib/fasttransforms.h"
// namespace
using namespace std;



class CrossCorrelator {
 private:
  // private containers
  DoubleSignal* original;
  vector<DoubleSignal*>* materials;
  vector<DoubleSignal*>* CCoriginals;
  map<pair<int,int>, DoubleSignal*>* CCmaterials;
 public:
  // constructors
  explicit CrossCorrelator(const string origPath, vector<string>& mPaths);
  // deleted copy- and assignment constructors
  CrossCorrelator(const CrossCorrelator& copyFrom)=delete;
  CrossCorrelator& operator= (const CrossCorrelator& copyFrom)=delete;
  // destructor
  ~CrossCorrelator();
  // getters
  DoubleSignal* getOriginal() const;
  vector<DoubleSignal*>* getMaterials() const;
  vector<DoubleSignal*>* getCCoriginals() const;
  map<pair<int,int>, DoubleSignal*>* getCCmaterials() const;
  DoubleSignal* getMaterial(const int i);
  DoubleSignal* getCCoriginal(const int i);
  DoubleSignal* getCCmaterial(const int i, const int j);
};

#endif
