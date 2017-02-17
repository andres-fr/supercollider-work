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
  string projectDir;
  DoubleSignal* original;
  vector<DoubleSignal*>* materials;
  double maxEnergy;
  SF_INFO* sf_orig;
  vector<DoubleSignal*>* CCoriginals;
  map<pair<int,int>, DoubleSignal*>* CCmaterials;
 public:
  // constructors
  explicit CrossCorrelator(// initialization values
                           const string ccd,
                           // needed by loadAudios()
                           const string origPath,
                           const vector<string>& matPaths,
                           const string metadata_name,
                           // needed by calculateCCs()
                           const unsigned int downSampleRatio);
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
  void loadAudiosSaveMetadata(const string origPath,
                              const vector<string>& matPaths,
                              const string metadata_name,
                              const unsigned int downSampleRatio);
  void calculateCCs(const unsigned int downSampleRatio);
};

#endif
