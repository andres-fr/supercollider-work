// stl includes
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
// other libraries
// own header
#include "wavsequence.h"
// namespaces
using namespace std;



////////////////////////////////////////////////////////////////////////////////
/// constructor&destructor
////////////////////////////////////////////////////////////////////////////////

WavSequence::WavSequence(const string pathIn, const string sep,
                         const string comm){
  separator = sep;
  commentMarker = comm;
  if (!pathIn.empty()){
    ifstream inList(pathIn.c_str());
    if(inList.is_open()==false){
      throw invalid_argument("WavSequence: Unable to open input file: "+pathIn);
    }
    string s; // temporary buffer holding infile lines of text
    while(getline(inList, s)){
      if(s.compare(0, comm.length(), comm)==0){continue;} // ignore comment lines
      int sepPos = s.find_first_of(sep);
      const int del = stoi(s.substr(0, sepPos));
      string rest = s.substr(sepPos+1);
      sepPos = rest.find_first_of(sep);
      const double norm = stod(rest.substr(0, sepPos));
      const string wavpath = rest.substr(sepPos+1);
      add(del, norm, wavpath);
    }
    inList.close();
  }
}

WavSequence::WavSequence(const string sep, const string comm)
  :WavSequence("", sep, comm){}

WavSequence::~WavSequence(){
}


////////////////////////////////////////////////////////////////////////////////
/// getters
////////////////////////////////////////////////////////////////////////////////
vector<wavsequence_entry> WavSequence::getContent(){
  return content;
}

////////////////////////////////////////////////////////////////////////////////
/// further functionality
////////////////////////////////////////////////////////////////////////////////


string WavSequence::asString() const{
  ostringstream oss;
  oss << commentMarker << " Content: <DELAY_IN_SAMPLES> " << separator <<
    " <NORMALIZATION_FACTOR> " << separator << " <PATH_TO_WAV_FILE>" << endl <<
    commentMarker << " fields are separated by '" << separator << "'" << endl <<
    commentMarker << " Lines starting with '" <<  commentMarker <<
    "' (comments) will be ignored" << endl;
  for (wavsequence_entry s : content){
      oss << s.delay  << separator<< s.k  << separator << s.wavPath << endl;
    }
  return oss.str();
}

void WavSequence::add(const int delay, const double k, const string wavPath){
  content.push_back(wavsequence_entry{delay, k, wavPath});
}

void WavSequence::toFile(const string pathOut){
  ofstream outFile(pathOut, ios::out);
  if(!outFile.is_open()){
    cout << "WavSequence: couldn't export to " << pathOut << ". Aborting"<<endl;
    return;
  } else{ // if able to open file
    cout << "WavSequence: writing to " << pathOut << endl;
    outFile << asString();
    outFile.close();
    cout << "wrote "<< pathOut << " succesfully!" << endl;
  }
}
