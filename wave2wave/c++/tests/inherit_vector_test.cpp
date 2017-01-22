// g++ -O3 -std=c++14 -Wall -Wextra /home/afr/git/supercollider-work/wave2wave/c++/tests/inherit_vector_test.cpp -o inherit_vector_test -lalglib -lsndfile && valgrind --leak-check=yes --track-origins=yes ./inherit_vector_test


#include <sndfile.hh>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include "libalglib/fasttransforms.h"
using namespace std;


class DoubleSignal : public alglib::real_1d_array {

public:
  const static bool AUTO_NORMALIZE = false;
  int delay;
  SF_INFO* sfInfo;
  explicit DoubleSignal(const int size, const double* d){
    delay = 0;
    sfInfo = new SF_INFO;
    sfInfo->format = 0;
    setcontent(size, d);
  };
  explicit DoubleSignal(const string wavPath){
    delay = 0;
    sfInfo = new SF_INFO;
    sfInfo->format = 0;
    // declare container for filestream open path to file
    SNDFILE* infile = sf_open(wavPath.c_str(), SFM_READ, sfInfo);
    // check that file opens
    if(infile == nullptr){
      throw invalid_argument("FloatSignal: Unable to open input file: "+wavPath);
    }
    // configure auto-normalization settings
    if (AUTO_NORMALIZE==false){
      sf_command(infile, SFC_SET_NORM_DOUBLE, nullptr, SF_FALSE);
    }
    // fit size and contents to the incoming file
    double* contents = new double[sfInfo->frames]();
    sf_read_double(infile, contents, sfInfo->frames);
    setcontent(sfInfo->frames, contents);
    sf_close(infile);
    delete[] contents;

  }

  ~DoubleSignal(){delete sfInfo;}

 // private:
 //  // signal fields
 //  float* contents;
 //  int sz;
 //  int delay;
 //  // wav client fields
 //  SF_INFO* sfInfo;
 //  static const int BLOCK_SIZE = 32;
 //  static const int AUTO_NORMALIZE = true;

 // public:
 //  // constructors and destructor
 //  explicit FloatSignal(int size) : FloatSignal(size, new float[size]()){};
 //  explicit FloatSignal(int size, float* conts);
 //  explicit FloatSignal(string wavPath);
 //  FloatSignal(const FloatSignal& copyFrom)=delete;
 //  FloatSignal& operator= (const FloatSignal& copyFrom)=delete;
 //  ~FloatSignal();
 //  //setters
 //  void setDelay(int d);
 //  void setSFInfo(SF_INFO sfi);
 //  // getters
 //  float* getContents() const;
 //  int getSize() const;
 //  int getDelay() const;
 //  SF_INFO* getSFInfo() const;
 //  // special getter (returns contents[idx-delay], never throws error)
 //  float at(int idx);

 //  // further functionality
 //  void normalize(float norm, float mean);
 //  void print(); // not affected by the delay
 //  void printSFInfo();<
 //  void toWav(string pathOut);



};


// FloatSignal::FloatSignal(int size, float* conts){
//   contents = conts;
//   sz = size;
//   delay = 0;
//   sfInfo = new SF_INFO;
//   sfInfo->format = 0;
// }

// FloatSignal::FloatSignal(string pathIn){
//   sz = 0;
//   delay = 0;
//   sfInfo = new SF_INFO;
//   sfInfo->format = 0;
//   // declare container for filestream open path to file

//   SNDFILE* infile = sf_open(pathIn.c_str(), SFM_READ, sfInfo);

//   // check that file opens
//   if(infile == nullptr){
//     throw invalid_argument("FloatSignal: Unable to open input file: "+pathIn);
//   }
//   // configure auto-normalization settings
//   if (!AUTO_NORMALIZE){
//     sf_command(infile, SFC_SET_NORM_FLOAT, nullptr, SF_FALSE);
//   }
//   sf_command(infile, SFC_SET_NORM_FLOAT, nullptr, SF_FALSE);
//   // fit size and contents to the incoming file
//   sz = sfInfo->frames;
//   contents = new float[sz]();
//   sf_read_float(infile, contents, sz);
//   sf_close(infile);
// }


// FloatSignal::~FloatSignal(){
//   delete[] contents;
//   delete sfInfo;
// }

// ////////////////////////////////////////////////////////////////////////////////
// // getters and setters
// ////////////////////////////////////////////////////////////////////////////////
// // setters
// void FloatSignal::setDelay(int d){delay=d;}
// void FloatSignal::setSFInfo(SF_INFO sfi){sfInfo= &sfi;}
// // getters
// float * FloatSignal::getContents() const {return contents;}
// int FloatSignal::getSize() const {return sz;}
// int FloatSignal::getDelay() const {return delay;}
// SF_INFO* FloatSignal::getSFInfo() const {return sfInfo;}
// // special getter
// float FloatSignal::at(int idx){
//   int di = idx-delay;
//   return (di<0 || di >= sz)? 0 : contents[di];
// }

// // void FloatSignal::set(int idx, float val){
// //   if (idx>=0 && idx < sz){contents[idx] = val;}
// //   else {throw invalid_argument ("FloatSignal::set: index "+
// //                                 to_string(idx)+" out of range!");}
// // }



// ////////////////////////////////////////////////////////////////////////////////
// // further functionality
// ////////////////////////////////////////////////////////////////////////////////

// void FloatSignal::print(){
//   cout <<"FloatSignal: [";
//   for (int i=0; i<(sz-1); i++){cout<<contents[i]<<", ";}
//   cout <<contents[sz-1]<<"]"<<endl;
// }

// void FloatSignal::printSFInfo(){
//   cout <<"sfInfo:"<<endl
//        << "\rframes: " << sfInfo->frames << endl
//        << "\rsamplerate: " << sfInfo->samplerate << endl
//        << "\rchannels: " << sfInfo->channels << endl
//        << "\rformat: " << sfInfo->format << endl
//        << "\rsections: " << sfInfo->sections << endl
//        << "\rseekable: " << sfInfo->seekable << endl;
// }


// void FloatSignal::normalize(float norm, float mean){
//   int i=0;
//   float sum = 0;
//   float max = 0;
//   // calculate mean into accum and get absolute maximum
//   for(i=0; i<sz; i++){
//     sum += contents[i];
//     if (abs(contents[i]) > abs(max)){
//       max = contents[i];
//     }
//   }
//   float deltamean = (sum/sz)-mean;
//   float deltanorm = norm / (max-deltamean);

//   // substract accum to each sample, and renormalize
//   for(i=0; i<sz; i++){
//     contents[i] = (contents[i]-deltamean)*deltanorm;
//   }
// }


// void FloatSignal::toWav(string pathOut){
//   // check that SF_INFO was configured
//   if(sfInfo == nullptr){
//     throw invalid_argument("FloatSignal.toWav(): sfInfo not configured");
//   }
//   // declare and try to open outfile
//   SNDFILE* outfile = nullptr;
//   if((outfile = sf_open(pathOut.c_str(), SFM_WRITE, sfInfo)) == nullptr){
//     throw invalid_argument("FloatSignal:Not able to open output file "+pathOut);
//   }
//   // configure auto-normalization settings
//   if (!AUTO_NORMALIZE){
//     sf_command(outfile, SFC_SET_NORM_FLOAT, nullptr, SF_FALSE);
//   }
//   // write signal contents (disregarding delay) to stream and close file
//   sf_write_float(outfile, contents, sz);
//   sf_close(outfile);
// }


int main(){

  // double* d = new double[10]();
  // DoubleSignal fs(10, d);
  // delete[] d;

  DoubleSignal fs("anvil.wav");
  cout << fs.length() << endl;
  cout << "test finished"<< endl;
  return 0;
}
