// stl includes
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <deque>
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
  delay = 0;
  delIdx = 0;
}

DoubleSignal::DoubleSignal(const string txtPath){
  ifstream inFile(txtPath.c_str());
  if(inFile.is_open()==false){
    throw invalid_argument("DoubleSignal: Unable to open input file: "+txtPath);
  } else{
    vector<double> v;
    double d = 0;
    while(inFile >> d){
      v.push_back(d);
    }
    
    // the -1 forces an error when calling toWav() with unconfigured sfInfo
    sfInfo = new SF_INFO;
    setSFInfo(-1,0,0,0,0,0);
    delay = 0;
    delIdx = 0;
    string asdf = txtPath;
    setcontent(v.size(), v.data());
    cout << "DoubleSignal: succesfully loaded " << txtPath << endl;
  }
}



DoubleSignal::DoubleSignal(const string wavPath, const bool autoNorm){
  delay = 0;
  delIdx = 0;
  sfInfo = new SF_INFO;
  setSFInfo(0,0,0,0,0,0); // initialize values for sfInfo(they are changed later)
  // declare container for filestream open path to file
  SNDFILE* infile = sf_open(wavPath.c_str(), SFM_READ, sfInfo);
  // check that file opens
  if(infile == nullptr){
    throw invalid_argument("DoubleSignal: Unable to open input file: "+wavPath);
  }
  // fit size and contents to the incoming file
  double* contents = new double[sfInfo->frames]();
  sf_read_double(infile, contents, sfInfo->frames);
  setcontent(sfInfo->frames, contents);
  if (autoNorm){normalizeArray(getcontent(), length(), 0, 1);}
  //close and finish
  sf_close(infile);
  delete[] contents;
}

// DoubleSignal::DoubleSignal(const int size){
//   delay = 0;
//   sfInfo = new SF_INFO;
//   sfInfo->format = 0;
//   double* arr = new double[size]();
//   setcontent(size, arr);
//   delete[] arr;
// };



////////////////////////////////////////////////////////////////////////////////
/// destructor
////////////////////////////////////////////////////////////////////////////////
DoubleSignal::~DoubleSignal(){
  delete sfInfo;
}


////////////////////////////////////////////////////////////////////////////////
/// setters
////////////////////////////////////////////////////////////////////////////////
void DoubleSignal::setDelay(int d){
  delay = d;
}

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
int DoubleSignal::getDelay() const {
  return delay;
}

SF_INFO* DoubleSignal::getSFInfo() const {
  return sfInfo;
}

// special getter
double DoubleSignal::at(int idx){
  delIdx = idx-delay;
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

void DoubleSignal::toWav(const string pathOut, const bool normalize){
  // check that SF_INFO was configured
  if(sfInfo->frames == -1){
    throw invalid_argument("DoubleSignal.toWav(): sfInfo not configured");
  }
  // declare and try to open outfile
  SNDFILE* outfile = sf_open(pathOut.c_str(), SFM_WRITE, sfInfo);
  if(outfile == nullptr){
    throw invalid_argument("DoubleSignal:Not able to open output file "+pathOut);
  }
  // configure auto-normalization settings
  if (normalize){
    // copy the contents to an array, normalize and copy :
    double* normArray = new double[length()];
    double* contents = getcontent();
    for (int i=0; i<length(); ++i){
      normArray[i] = contents[i];
    }
    // normalize andwrite signal contents (disregarding delay) to stream
    normalizeArray(normArray, length(), 0, 1);
    sf_write_double(outfile, normArray, length());
    // delete array and close stream
    delete[] normArray;
    sf_close(outfile);
    return;
  
  } else {
    // write signal contents (disregarding delay) to stream and close file
    sf_write_double(outfile, getcontent(), length());
    sf_close(outfile);
    return;
  }
}


bool DoubleSignal::checkSRateAndChans(const SF_INFO* sf2){
  const SF_INFO* sf1 = getSFInfo();
  return (sf1->samplerate == sf2->samplerate && sf1->channels == sf2->channels);
}


void DoubleSignal::toRaw(const string pathOut){
  deque<string> v;
  double* arr = getcontent();
  for (int i=0; i<length(); ++i){
    v.push_back(to_string(arr[i]));
  }
  ofstream outFile(pathOut.c_str(), ios::out);
  if (outFile.is_open()){
    cout << "toRaw: writing "<< length() <<" elements to " << pathOut << endl;
    int globalIter = 0;
    while (globalIter<(length()-1)){
      string bufStr = "";
      int bufIter = 0;
      while(bufIter<1000 && globalIter<(length()-1)){
        bufStr += v[globalIter] +"\n";
        bufIter++;
        globalIter++;
      }
      outFile << bufStr;
    }
    outFile << v[globalIter];
    outFile.close();
    cout << "toRaw: wrote succesfully!" << endl;
  } else{
    cout << "toRaw: could't open " << pathOut << endl;
  }
}
