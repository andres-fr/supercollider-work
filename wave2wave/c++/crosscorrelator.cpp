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

CrossCorrelator::CrossCorrelator(const string wsDir, const string ccd)
  : workspaceDir(wsDir), ccDir(ccd), maxEnergy(0){}

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



////////////////////////////////////////////////////////////////////////////////
//// OTHER FUNCTIONALITY
////////////////////////////////////////////////////////////////////////////////

void CrossCorrelator::loadAudios(const string origPath,
                                 const vector<string>& matPaths,
                                 const string metadata_name){
  // load original
  original = new DoubleSignal(origPath, true);
  maxEnergy = (original->energy()>maxEnergy)? original->energy() : maxEnergy;
  sf_orig = original->getSFInfo();
  // instantiate and load materials
  materials = new vector<DoubleSignal*>;
  for (string path : matPaths){
    DoubleSignal* m = new DoubleSignal(path, true);
    if (m->checkSRateAndChans(sf_orig)==false){
      SF_INFO* sfm = m->getSFInfo();
      cout << "~warning: in CrossCorrelator: incompatible srate/nChans" <<
        endl << origPath <<": ("<<  sf_orig->samplerate <<", "<< sf_orig->channels <<")"<<
        endl << workspaceDir+"AUDIO/"+path <<": ("<< sfm->samplerate <<", "<<
        sfm->channels <<")";
    }
    materials->push_back(m);
    maxEnergy = (m->energy()>maxEnergy)? m->energy() : maxEnergy;
  }
  // save METADATA: wavPath, length, energy
  ofstream outFile((workspaceDir+metadata_name).c_str(), ios::out);
  if(outFile.is_open()){
    cout << "writing "<< metadata_name <<" file" << endl;
    outFile << origPath <<":"<< original->length() <<":"<< original->energy();
    for(unsigned int i=0; i<materials->size(); ++i){
      outFile << endl << workspaceDir << "AUDIO/" << matPaths.at(i) << ":" <<
        materials->at(i)->length() << ":" << materials->at(i)->energy();
    }
    outFile.close();
    cout << "wrote "<< workspaceDir << metadata_name <<" succesfully!" << endl;
  }



}


void CrossCorrelator::calculateCCs(unsigned int downSampleRatio){

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
    cc->multiplyBy(1.0/maxEnergy);
    // configure cc->sfInfo
    cc->setSFInfo(cc->length(), sf_orig->samplerate, sf_orig->channels,
                  sf_orig->format, sf_orig->sections, sf_orig->seekable);
    // export to unnormalized .wav (already normalized with .multiplyBy)
    unsigned int offset = (m->length()-1)%downSampleRatio; // this enables the zero-idx to be always present
    cc->toWav(ccDir+"ANALYSIS/cc_original_m"+
              to_string(CCoriginals->size()-1)+ ".wav",false,
              downSampleRatio, offset);
  }

  // instantiate and load CCmaterials
  int N = materials->size(); // to save some time
  CCmaterials = new map<pair<int,int>, DoubleSignal*>;
  for (int i=0; i<N; ++i){ // "static"
    DoubleSignal* mi = (*materials)[i]; // to save some ns
    unsigned int offset = (mi->length()-1)%downSampleRatio; // this enables the zero-idx to be always present
    for (int j=i; j<N; ++j){ // "shifter"
      DoubleSignal* mj = (*materials)[j]; // not sure if this saves time
      DoubleSignal* cc = new DoubleSignal;
      // compute CC[mi,mj] and add to DS
      cout << "calculating CCmaterials..." << endl;
      alglib::corrr1d(*mi, mi->length(), *mj, mj->length(), *cc);
      (*CCmaterials)[make_pair(i,j)] = cc;
      //normalize array: highest peak of all CC is 1, the rest is proportional
      cc->multiplyBy(1.0/maxEnergy);
      // rotate the arr. to be in format [-(M-1) ... 0 ... (N-1)]
      rotateArray(cc->getcontent(), cc->length(), mi->length());
      // configure cc->sfInfo
      cc->setSFInfo(cc->length(), sf_orig->samplerate, sf_orig->channels,
                    sf_orig->format, sf_orig->sections, sf_orig->seekable);
      // export to unnormalized .wav (already normalized with .multiplyBy)
      cc->toWav(ccDir+"ANALYSIS/cc_m"+to_string(i)+"_m"+to_string(j)+
                ".wav", false, downSampleRatio, offset);
    }
  }

}
