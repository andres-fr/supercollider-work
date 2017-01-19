// original file from http://stackoverflow.com/questions/19377942/processing-large-files-with-libsndfile
// g++ -std=c++14 -Wall -pedantic -Wextra <C++ FILE PATH> -o MAIN -l sndfile && ./MAIN
// valgrind --leak-check=yes ./MAIN


#include <sndfile.hh>
#include <iostream>
#include <libalglib/fasttransforms.h>
using namespace std;

// global config
#define BLOCK_SIZE 32
string fileName1 = "child-short.wav";
string fileName2 = "anvil.wav";
string fileOut = "output.wav";


int main(){

  // locals
  SNDFILE *infile1  = NULL;
  SNDFILE *infile2  = NULL;
  SNDFILE *outfile = NULL;
  SF_INFO    sfinfo1;
  SF_INFO    sfinfo2;
  SF_INFO    sfinfoOut;
  sf_count_t readcount;
  short BufferIn[BLOCK_SIZE];
  short BufferOut[BLOCK_SIZE];



  // open original and display info
  cout << "Trying to open " << fileName1 <<  endl;
  if((infile1 = sf_open(fileName1.c_str(),SFM_READ,&sfinfo1)) == NULL){
      printf("Not able to open input file 1\n");
      printf("\n\nPress any key to exit.");
      puts(sf_strerror(NULL)) ;
      return 1;
    }
  cout << "succesfully opened" <<  endl << "Channels: " << sfinfo1.channels <<
    endl << "Frames: " << sfinfo1.frames << endl << "Samplerate: "
       << sfinfo1.samplerate  <<  endl;



  // open material and display info
  cout << "Trying to open " << fileName2 <<  endl;
  if((infile2 = sf_open(fileName2.c_str(),SFM_READ,&sfinfo2)) == NULL) {
      printf("Not able to open input file 1\n");
      printf("\n\nPress any key to exit.");
      puts(sf_strerror(NULL)) ;
      return 1;
    }
  cout << "succesfully opened" <<  endl << "Channels: " << sfinfo2.channels <<
    endl << "Frames: " << sfinfo2.frames << endl << "Samplerate: "
       << sfinfo2.samplerate  <<  endl;



  // check configurations and abort if not compatible
  if(sfinfo1.channels != sfinfo2.channels){
      std::cout << "Channelammount differs from eachother, abort program.";
      printf("\n\nPress any key to exit.");
      return 1;
    }
  if(sfinfo1.samplerate != sfinfo2.samplerate){
      std::cout << "Samplerate differs from eachother, abort program.";
      printf("\n\nPress any key to exit.");
      return 1;
    }


  // configure and open fileOut
  sfinfoOut.channels = sfinfo1.channels;
  sfinfoOut.format = sfinfo1.format;
  sfinfoOut.samplerate = sfinfo1.samplerate;
  cout << "Trying to open output " << fileOut <<  endl;
  if((outfile = sf_open(fileOut.c_str(),SFM_WRITE,&sfinfoOut)) == NULL){
      printf("Not able to create output file \n");
      printf("\n\nPress any key to exit.");
      puts(sf_strerror(NULL)) ;
      return 1;
    }
  cout << "succesfully opened" <<  endl;


  // PROCESS
  cout << "writing to output..." <<  endl;
  // sf_read_short returns the number of samples it read (stops at EOF)
  while((readcount = sf_read_short(infile1, BufferIn, BLOCK_SIZE)))
    { // copy the contents from FileIn to fileOut
      for(int i = 0; i < readcount; i++) {BufferOut[i] = BufferIn[i];}
      for(int i = readcount; i < BLOCK_SIZE; i++){BufferOut[i] = 0;}
      // append the contents to the current end of the file
      sf_write_short(outfile, BufferOut, BLOCK_SIZE) ;
    }


  // close streams and finish program
  sf_close(infile1);
  sf_close(infile2);
  sf_close(outfile);
  cout << "program finished" <<  endl;
  return 0;
}






// class CCS
// {
// public:
//   Cat(int initialAge);
//   Cat(const Cat& copy_from) = delete; //forbid copy constructor
//   Cat& operator=(const Cat& copy_from) = delete; //fobid copy assignment
//   ~Cat();
//   //
//   int GetAge() const;            // accessor function
//   void SetAge(int age);    // accessor function
//   void Meow();
// private:                   // begin private section
//   int itsAge;              // member variable
//   char * string;
// };


// Cat::Cat(int initialAge)
// {
//   itsAge = initialAge;
//   string = new char[10]();
// }

// Cat::~Cat()
// {
//     delete[] string;
// }

// int Cat::GetAge() const {return itsAge;}
// void Cat::SetAge(int age) {itsAge = age;}

// void Cat::Meow(){cout << "Meow.\n";}

// int main(){
//   int Age;
//   cout<<"How old is Frisky? ";
//   cin>>Age;
//   Cat Frisky(Age);
//   Frisky.Meow();
//   cout << "Frisky is a cat who is "<< Frisky.GetAge() << " years old."<< endl;
//   Frisky.Meow();
//   Age++;
//   Frisky.SetAge(Age);
//   cout << "Now Frisky is " << Frisky.GetAge() << " years old."<< endl;
//   return 0;
// }




// el plan es:
// 1) calcular la FFTCC entre 2 wavs y ver q aspecto tiene
// 2)
