// stl includes
#include<iostream>
#include<fstream>
#include<vector>
#include <map>
// other libraries
#include "doublesignal.h"
#include "libalglib/fasttransforms.h"
// own dependencies
#include "freefunctions.h"
// own header
#include "crosscorrelator.h"
// namespace
using namespace std;

CrossCorrelator::CrossCorrelator(const string origName,
                                 const vector<string>& mNames,
                                 const string workingDir,
                                 const unsigned int downSampleRatio){
  //
  const string METADATA_NAME = "METADATA.txt";
  // input parameter preprocessing
  const string origPath = workingDir+"AUDIO/"+origName;
  // store the max energy to normalize all CCs dividing through it
  double max_energy = 0;
  // load original
  original = new DoubleSignal(origPath, true);
  max_energy = (original->energy()>max_energy)? original->energy() : max_energy;
  SF_INFO* sf = original->getSFInfo();
  // instantiate and load materials
  materials = new vector<DoubleSignal*>;
  for (string path : mNames){
    DoubleSignal* m = new DoubleSignal(workingDir+"AUDIO/"+path, true);
    if (m->checkSRateAndChans(sf)==false){
      SF_INFO* sfm = m->getSFInfo();
      cout << "~warning: in CrossCorrelator: incompatible srate/nChans" <<
        endl << origPath <<": ("<<  sf->samplerate <<", "<< sf->channels <<")"<<
        endl << workingDir+"AUDIO/"+path <<": ("<< sfm->samplerate <<", "<<
        sfm->channels <<")";
    }
    materials->push_back(m);
    max_energy = (m->energy()>max_energy)? m->energy() : max_energy;
  }
  // save METADATA: wavPath, length, energy
  ofstream outFile((workingDir+METADATA_NAME).c_str(), ios::out);
  if(outFile.is_open()){
    cout << "writing "<< METADATA_NAME <<" file" << endl;
    outFile << origPath <<":"<< original->length() <<":"<< original->energy();
    for(unsigned int i=0; i<materials->size(); ++i){
      outFile << endl << workingDir << "AUDIO/" << mNames.at(i) << ":" <<
        materials->at(i)->length() << ":" << materials->at(i)->energy();
    }
    outFile.close();
    cout << "wrote "<< workingDir << METADATA_NAME <<" succesfully!" << endl;
  }

  // instantiate and load CCoriginals
  CCoriginals = new vector<DoubleSignal*>;
  for (DoubleSignal* m : *materials){
    // instantiate container for CC
    DoubleSignal* cc = new DoubleSignal;
    // compute CC[original, m] and add to DS
    cout << "calculating CCoriginals..." << endl;
    alglib::corrr1d(*original, original->length(), *m, m->length(), *cc);
    CCoriginals->push_back(cc);
    // rotate the arr. to be in format [-(M-1) ... 0 ... (N-1)]
    rotateArray(cc->getcontent(), cc->length(), original->length());
    // normalize array: highest peak of all CC is 1, the rest is proportional
    cc->multiplyBy(1.0/max_energy);
    // configure cc->sfInfo
    cc->setSFInfo(cc->length(), sf->samplerate, sf->channels,
                  sf->format, sf->sections, sf->seekable);
    // export to unnormalized .wav (already normalized with .multiplyBy)
    cc->toWav(workingDir+"ANALYSIS/"+"cc_original_m"+
              to_string(CCoriginals->size()-1)+ ".wav",false, downSampleRatio);
  }

  // instantiate and load CCmaterials
  int N = materials->size(); // to save some time
  CCmaterials = new map<pair<int,int>, DoubleSignal*>;
  for (int i=0; i<N; ++i){
    DoubleSignal* mi = (*materials)[i]; // to save some ns
    for (int j=i; j<N; ++j){
      DoubleSignal* mj = (*materials)[j]; // not sure if this saves time
      DoubleSignal* cc = new DoubleSignal;
      // compute CC[mi,mj] and add to DS
      cout << "calculating CCmaterials..." << endl;
      alglib::corrr1d(*mi, mi->length(), *mj, mj->length(), *cc);
      (*CCmaterials)[make_pair(i,j)] = cc;
      //normalize array: highest peak of all CC is 1, the rest is proportional
      cc->multiplyBy(1.0/max_energy);
      // rotate the arr. to be in format [-(M-1) ... 0 ... (N-1)]
      rotateArray(cc->getcontent(), cc->length(), mi->length());
      // configure cc->sfInfo
      cc->setSFInfo(cc->length(), sf->samplerate, sf->channels,
                    sf->format, sf->sections, sf->seekable);
      // export to unnormalized .wav (already normalized with .multiplyBy)
      cc->toWav(workingDir+"ANALYSIS/"+"cc_m"+to_string(i)+"_m"+to_string(j)+
                ".wav", false, downSampleRatio);
    }
  }
}

CrossCorrelator::~CrossCorrelator(){

  // delete original
  delete original;

  // delete materials
  for (DoubleSignal* ds : *materials){
    delete ds;
  }delete materials;

  // delete CCoriginals
  for (DoubleSignal* ds : *CCoriginals){
    delete ds;
  }delete CCoriginals;

  // delete CCmaterials
  for (pair<pair<int,int>, DoubleSignal*> kv : *CCmaterials){
    delete kv.second;
  }delete CCmaterials;

}

DoubleSignal* CrossCorrelator::CrossCorrelator::getOriginal() const {
  return original;
}

vector<DoubleSignal*>* CrossCorrelator::getMaterials() const {
  return materials;
}


vector<DoubleSignal*>* CrossCorrelator::getCCoriginals() const {
  return CCoriginals;
}


map<pair<int,int>, DoubleSignal*>* CrossCorrelator::getCCmaterials() const {
  return CCmaterials;
}

DoubleSignal* CrossCorrelator::getMaterial(const int i){
  return (*materials).at(i);
}

DoubleSignal* CrossCorrelator::getCCoriginal(const int i){
  return (*CCoriginals).at(i);
}

DoubleSignal* CrossCorrelator::getCCmaterial(const int i, const int j){
  if (i<j){
    return (*CCmaterials)[make_pair(i,j)];
  } else {
    return (*CCmaterials)[make_pair(j,i)];
  }
}
