// original file from http://stackoverflow.com/questions/19377942/processing-large-files-with-libsndfile
// g++ -std=c++14 -Wall -pedantic -Wextra <C++ FILE PATH> -o MAIN -l sndfile && ./MAIN

#include <sndfile.hh>
#include <iostream>
#include <fstream>

#define BLOCK_SIZE 32

using namespace std;

int main()

{
  SNDFILE *infile  = NULL;
  SNDFILE *outfile = NULL;
  SF_INFO    sfinfo;
  SF_INFO    sfinfoOut;
  sf_count_t readcount;
  short BufferIn[BLOCK_SIZE];
  short BufferOut[BLOCK_SIZE];

  string FileIn = <INPUT WAV FILE PATH WITH EXTENSION>;
  string FileOut = <OUTPUT WAV FILE PATH WITH EXTENSION>;


  // open FileIn and display info
  cout << "Trying to open " << FileIn <<  endl;
  if((infile = sf_open(FileIn.c_str(),SFM_READ,&sfinfo)) == NULL)
    {
      printf("Not able to open input file 1\n");
      printf("\n\nPress any key to exit.");
      puts(sf_strerror(NULL)) ;
      return 1;
    }
  cout << "succesfully opened" <<  endl;
  cout << "Channels: " << sfinfo.channels <<
    "\nFrames: " << sfinfo.frames << endl;
  cout << "Samplerate: " << sfinfo.samplerate  <<  endl;


  // configure and open FileOut
  sfinfoOut.channels = sfinfo.channels;
  sfinfoOut.format = sfinfo.format;
  sfinfoOut.samplerate = sfinfo.samplerate;
  cout << "Trying to open output " << FileOut <<  endl;
  if((outfile = sf_open(FileOut.c_str(),SFM_WRITE,&sfinfoOut)) == NULL)      // Open Wav-file 2 instance
    {
      printf("Not able to create output file \n");
      printf("\n\nPress any key to exit.");                      // Exit-text if file not present
      puts(sf_strerror(NULL)) ;
      return 1;
    }
  cout << "succesfully opened" <<  endl;


  //
  cout << "writing to output..." <<  endl;
  // sf_read_short returns the number of samples it read (stops at EOF)
  while((readcount = sf_read_short(infile, BufferIn, BLOCK_SIZE)))
    { // copy the contents from FileIn to FileOut
      for(int i = 0; i < readcount; i++) {BufferOut[i] = BufferIn[i];}
      for(int i = readcount; i < BLOCK_SIZE; i++){BufferOut[i] = 0;}
      // append the contents to the current end of the file
      sf_write_short(outfile, BufferOut, BLOCK_SIZE) ;
    }
  sf_close(infile);                                                    // Close Wav-file handlers
  sf_close(outfile);
  cout << "program finished" <<  endl;
  return 0;
}
