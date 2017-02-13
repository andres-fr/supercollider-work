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
  string workspaceDir;
  string ccDir;
  DoubleSignal* original;
  vector<DoubleSignal*>* materials;
  double maxEnergy;
  SF_INFO* sf_orig;
  vector<DoubleSignal*>* CCoriginals;
  map<pair<int,int>, DoubleSignal*>* CCmaterials;
 public:
  // constructors
  explicit CrossCorrelator(const string workspaceDir, const string ccDir);
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
  // other functionality
  void loadAudios(const string origPath, const vector<string>& matPaths, const string metadata_name);
  void calculateCCs(unsigned int downSampleRatio);
};

#endif
