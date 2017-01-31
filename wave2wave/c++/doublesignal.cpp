// stl includes
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <deque>
#include<algorithm>
// other libraries
#include "sndfile.hh"
#include "libalglib/ap.h"
// own dependencies
#include "freefunctions.h"
// own header
#include "doublesignal.h"
// namespaces
using namespace std;



////////////////////////////////////////////////////////////////////////////////
/// constructors
////////////////////////////////////////////////////////////////////////////////
DoubleSignal::DoubleSignal() : alglib::real_1d_array(){
  sfInfo = new SF_INFO;
  // the -1 forces an error when calling toWav() with unconfigured sfInfo
  setSFInfo(-1,0,0,0,0,0); // ds.setSFInfo(ds.length(), 44100, 1, 65538, 1, 1);
  delIdx = 0;
}

DoubleSignal::DoubleSignal(int size) : alglib::real_1d_array(){
  sfInfo = new SF_INFO;
  // the -1 forces an error when calling toWav() with unconfigured sfInfo
  setSFInfo(-1,0,0,0,0,0); // ds.setSFInfo(ds.length(), 44100, 1, 65538, 1, 1);
  delIdx = 0;
  double* d = new double[size]();
  setcontent(size, d);
  delete d;
}

DoubleSignal::DoubleSignal(const string txtPath){
  // try to open given path
  ifstream inFile(txtPath.c_str());
  if(inFile.is_open()==false){
    throw invalid_argument("DoubleSignal txt constructor: Unable to open input file: "+txtPath);
  } else{
    // if file opens, load numbers, one double per line
    vector<double> v;
    double d = 0;
    while(inFile >> d){
      v.push_back(d);
    } // finally set vector
    setcontent(v.size(), v.data());
    // initialize other fields
    sfInfo = new SF_INFO;
    setSFInfo(-1,0,0,0,0,0);
    delIdx = 0;
    // print and finish
    cout <<"DoubleSignal txt constructor: succesfully loaded "<< txtPath<< endl;
  }
}



DoubleSignal::DoubleSignal(const string wavPath, const bool autoNorm){
  // inizialize fields
  delIdx = 0;
  sfInfo = new SF_INFO;
  setSFInfo(0,0,0,0,0,0); // values are properly changed later by sf_open
  // try to open given path
  SNDFILE* infile = sf_open(wavPath.c_str(), SFM_READ, sfInfo);
  if(infile == nullptr){
    throw invalid_argument("DoubleSignal wav constructor: Unable to open input file: "+wavPath);
  } else{ // if file opens...
    // // library default-normalizes to -1...1. Disable that!
    // sf_command (infile, SFC_SET_NORM_DOUBLE, nullptr, SF_FALSE);
    // get file contents and save them to array
    double* contents = new double[sfInfo->frames]();
    sf_read_double(infile, contents, sfInfo->frames);
    setcontent(sfInfo->frames, contents);
    // optional: custom normalization
    if (autoNorm){normalizeArray(getcontent(), length(), 0, 1);}
    //close, delete, print and finish
    sf_close(infile);
    delete[] contents;
    cout <<"DoubleSignal wav constructor: succesfully loaded "<< wavPath<< endl;
  }
}



////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////
DoubleSignal::~DoubleSignal(){
  delete sfInfo;
}


////////////////////////////////////////////////////////////////////////////////
/// setters
////////////////////////////////////////////////////////////////////////////////
void DoubleSignal::setSFInfo(const sf_count_t fr, const int sr,
               const int chans, const int formt,
               const int sect, const int seek){
  sfInfo->frames = fr;
  sfInfo->samplerate = sr;
  sfInfo->channels = chans;
  sfInfo->format = formt;
  sfInfo->sections = sect;
  sfInfo->seekable = seek;
}


////////////////////////////////////////////////////////////////////////////////
/// getters
////////////////////////////////////////////////////////////////////////////////
SF_INFO* DoubleSignal::getSFInfo() const {
  return sfInfo;
}

// special getter: takes delay field into account
double DoubleSignal::at(int idx, int del){
  delIdx = idx-del;
  if(delIdx<0 || delIdx >= length()){
    return 0;
  } else{
    return (*this)[delIdx];
  }
}

////////////////////////////////////////////////////////////////////////////////
/// other functionality
////////////////////////////////////////////////////////////////////////////////
void DoubleSignal::printSFInfo(){
  cout <<"==sfInfo=="<<endl
       << "\rframes: " << sfInfo->frames << endl
       << "\rsamplerate: " << sfInfo->samplerate << endl
       << "\rchannels: " << sfInfo->channels << endl
       << "\rformat: " << sfInfo->format << endl
       << "\rsections: " << sfInfo->sections << endl
       << "\rseekable: " << sfInfo->seekable << endl;
}




bool DoubleSignal::checkSRateAndChans(const SF_INFO* sf2){
  const SF_INFO* sf1 = getSFInfo();
  return (sf1->samplerate == sf2->samplerate && sf1->channels == sf2->channels);
}


double DoubleSignal::energy(){
  return inner_product(getcontent(), getcontent()+length(), getcontent(), 0.0);
}

void DoubleSignal::multiplyBy(const double x){
  double* arr = getcontent();
  for(int i=0; i<length(); ++i){
    arr[i] *= x;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// exporting
////////////////////////////////////////////////////////////////////////////////

void DoubleSignal::toASCII(const string pathOut){
  // declare and try to open outFile
  ofstream outFile(pathOut.c_str(), ios::out);
  if (!outFile.is_open()){
    cout << "toRaw: could't open " << pathOut << endl;
    return;
  } else{// if file opened succesfully...
    // fill a deque with contents of double* as strings
    deque<string> v;
    double* arr = getcontent();
    for (int i=0; i<length(); ++i){
      v.push_back(to_string(arr[i]));
    }
    // writing to stream PACKET_SIZE elements each time minimizes I/O calls
    const int PACKET_SIZE = 1000;
    cout << "toRaw: writing "<< length() <<" elements to " << pathOut << endl;
    int globalIter = 0; // iterates once from 0 to length()-2 (see ** below)
    while (globalIter<(length()-1)){ // global loop
      string bufStr = "";
      int bufIter = 0; // iterates many times from 0 to PACKET_SIZE-1
      while(bufIter<PACKET_SIZE && globalIter<(length()-1)){ // packet loop
        bufStr += v[globalIter] +"\n";
        bufIter++;
        globalIter++;
      }
      outFile << bufStr; // write packet
    }
    // ** add v[length()-1] (without trailing \n), close stream, print, return
    outFile << v[globalIter]; 
    outFile.close();
    cout << "toRaw: succesfully saved to " << pathOut << endl;
  }
}


void DoubleSignal::toWav(const string pathOut, const bool norm){
  // // append _RAW if not normalizing, for clarification
  // string pathOut = path_out;
  // if (!normalize){pathOut += "_RAW";}
  // check that SF_INFO was configured
  if(sfInfo->frames == -1){
    throw invalid_argument("toWav: sfInfo not configured");
  }
  // declare and try to open outfile
  SNDFILE* outfile = sf_open(pathOut.c_str(), SFM_WRITE, sfInfo);
  if(outfile == nullptr){
    throw invalid_argument("toWav: Not able to open output file "+pathOut);
  } else{// if file opens...
    if (norm){ // if (normalize==true)... ***
      // copy contents to an array, and normalize that array:
      double* normArray = new double[length()];
      double* contents = getcontent();
      for (int i=0; i<length(); ++i){normArray[i] = contents[i];}
      normalizeArray(normArray, length(), 0, 1); // 0 mean, 1 abs(max)
      // write normalized contents to stream (disregarding delay)
      sf_write_double(outfile, normArray, length());
      // delete normArray, close stream, print and return 
      delete[] normArray;
      sf_close(outfile);
      cout  << "toWav: succesfully saved to "<< pathOut << endl;
      return;
    } else { // if NOT normalize:
      // write contents to stream (disregarding delay), close, print and return
      sf_write_double(outfile, getcontent(), length());
      sf_close(outfile);
      cout  << "toWav: succesfully saved to "<< pathOut << endl;
      return;
    }
  }
}
