// stl includes
#include<iostream>
#include<fstream>
#include<vector>
#include <map>
// other libraries
#include "floatsignal.h"
#include "libalglib/fasttransforms.h"
// own dependencies
#include "freefunctions.h"
// own header
#include "crosscorrelator.h"
// namespace
using namespace std;

CrossCorrelator::CrossCorrelator(const string origPath,
                                 const vector<string>& mPaths,
                                 const string outFolder){
  // store the max energy to normalize all CCs dividing through it
  float max_energy = 0;
  // load original
  original = new FloatSignal(origPath, true);
  SF_INFO* sf = original->getSFInfo();
  float orig_energy = original->energy();
  if (orig_energy > max_energy){
    max_energy = orig_energy;
  }
  // instantiate and load materials
  materials = new vector<FloatSignal*>;
  for (string path : mPaths){
    FloatSignal* m = new FloatSignal(path, true);
    SF_INFO* sfm = m->getSFInfo();
    if (sf->samplerate!=sfm->samplerate || sf->channels!=sfm->channels){
      cout << "~warning: in CrossCorrelator: incompatible srate/nChans" <<
        endl << origPath <<": ("<<  sf->samplerate <<", "<< sf->channels <<")"<<
        endl << path <<": ("<< sfm->samplerate <<", "<< sfm->channels <<")";
    }
    materials->push_back(m);
    float m_energy = m->energy();
    if (m_energy > max_energy){
      max_energy = m_energy;
  }
  }
  // try to save METADATA.txt if some out-dir given (if not just load in RAM)
  if (!outFolder.empty()){
    ofstream outFile((outFolder+METADATA_NAME).c_str(), ios::out);
    if(outFile.is_open()){ // if the given out-dir works:
      cout << "writing " << METADATA_NAME << endl;
      outFile << origPath <<":"<< original->length() <<":"<< original->energy();
      for(unsigned int i=0; i<materials->size(); ++i){
        outFile << endl << mPaths.at(i) << ":" << materials->at(i)->length()
                << ":" << materials->at(i)->energy();
      }
      outFile.close();
      cout << "succesfully wrote "<< outFolder << METADATA_NAME << endl;
    } else{ // if the given out-dir didn_t't work
      cout << "could't open " << outFolder << METADATA_NAME << endl;
    }
  }
  // this datatype conversion is needed by corrr1d:
  alglib::real_1d_array alg_orig;
  alg_orig.setcontent(original->length(), original->data());

  // instantiate and load CCoriginals
  CCoriginals = new vector<FloatSignal*>;
  for (FloatSignal* m : *materials){
    // this datatype conversion is needed by corrr1d:
    alglib::real_1d_array alg_m;
    alg_m.setcontent(m->length(), m->data());
    // instantiate container for CC
    alglib::real_1d_array cc;
    // compute CC[original, m] and add to DS
    cout << "calculating CCoriginals..." << endl;
    alglib::corrr1d(alg_orig, alg_orig.length(), alg_m, alg_m.length(), cc);
    // now convert back calculated CC from real_1d_array to FloatSignal
    FloatSignal* fs_cc = new FloatSignal(cc.length());
    copy(cc.getcontent(), cc.getcontent()+cc.length(), fs_cc->data());
    CCoriginals->push_back(fs_cc);
    // rotate the arr. to be in format [-(M-1) ... 0 ... (N-1)]
    rotateArray(fs_cc->data(), fs_cc->length(), original->length());
    // NORMALIZE DIVIDING BY MAX_ENERGY (to be able to efficiently store as wav)
    fs_cc->multiplyBy(1.0/max_energy); // ***
    // configure cc->sfInfo
    fs_cc->setSFInfo(fs_cc->length(), sf->samplerate, sf->channels,
                  sf->format, sf->sections, sf->seekable);
    // export to outFolder as wave if output path was given
    if (!outFolder.empty()){
      fs_cc->toWav(outFolder+"cc_original_m"+to_string(CCoriginals->size()-1)+
                   ".wav", false); // already normalized in ***
    }
  }

  // instantiate and load CCmaterials
  int N = materials->size(); // to save some time
  CCmaterials = new map<pair<int,int>, FloatSignal*>;
  for (int i=0; i<N; ++i){
    FloatSignal* mi = (*materials)[i]; // to save some ns
    // this datatype conversion is needed by corrr1d:
      alglib::real_1d_array alg_mi;
      alg_mi.setcontent(mi->length(), mi->data());
    for (int j=i; j<N; ++j){
      FloatSignal* mj = (*materials)[j]; // not sure if this saves time



      FloatSignal* cc = new FloatSignal;
      // compute CC[mi,mj] and add to DS
      cout << "calculating CCmaterials..." << endl;
      alglib::corrr1d(*mi, mi->length(), *mj, mj->length(), *cc);
      (*CCmaterials)[make_pair(i,j)] = cc;
      // rotate the arr. to be in format [-(M-1) ... 0 ... (N-1)]
      rotateArray(cc->getcontent(), cc->length(), mi->length());
      // configure cc->sfInfo
      cc->setSFInfo(cc->length(), sf->samplerate, sf->channels,
                    sf->format, sf->sections, sf->seekable);
      // export to .txt if output path was given
      if (!outFolder.empty()){
        cc->toASCII(outFolder+"cc_m"+to_string(i)+"_m"+to_string(j)+".txt");
      }
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
