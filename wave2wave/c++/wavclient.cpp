#include <sndfile.hh>
#include <vector>
#include <stdexcept>
#include "wavclient.h"
using namespace std;



vector<float> WavClient::wavPath2floatVector(string pathIn){
    // declare containers for filestream and its metadata, open path to file
    SF_INFO sfinfoIn;
    SNDFILE* infile = sf_open(pathIn.c_str(), SFM_READ, &sfinfoIn);
    // check that file opens, and has suitable features
    if(infile == nullptr){
      throw invalid_argument("WavClient: Not able to open input file: "+pathIn);
    }
    if(sfinfoIn.samplerate!=SAMPLE_RATE || sfinfoIn.channels!=NUM_CHANNELS
       || sfinfoIn.format!=FORMAT){
      throw invalid_argument ("WavClient: config error.\nsample rate: "+
                              to_string(sfinfoIn.samplerate)+"\nchannels: "+
                              to_string(sfinfoIn.channels)+"\nformat:"+
                              to_string(sfinfoIn.format));
    }
    // create array with length of wavfile, fill it, close file
    float * wavRaw = new float[sfinfoIn.frames]();
    sf_read_float(infile, wavRaw, sfinfoIn.frames);
    sf_close(infile);
    // pass the array contents to vector, delete array and return
    vector<float> v;
    v.assign(wavRaw, wavRaw+sfinfoIn.frames);
    delete[] wavRaw;
    return v;
  }

void WavClient::floatVector2wav(string pathOut, const vector<float> &wavRaw){
    // declare containers for filestream and its metadata
    SNDFILE* outfile = nullptr;
    SF_INFO sfinfoOut;
    // set class config to metadata
    sfinfoOut.channels = NUM_CHANNELS;
    sfinfoOut.samplerate = SAMPLE_RATE;
    sfinfoOut.format = FORMAT;
    // safely open stream
    if((outfile = sf_open(pathOut.c_str(), SFM_WRITE, &sfinfoOut)) == nullptr){
      throw invalid_argument("WavClient:Not able to open output file "+pathOut);
    }
    // write vector contents to stream and close file
    sf_write_float(outfile, &wavRaw[0], wavRaw.size());
    sf_close(outfile);
  }
