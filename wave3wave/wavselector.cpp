// stl includes
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
// other libraries
// own header
#include "wavselector.h"
// namespaces
using namespace std;



////////////////////////////////////////////////////////////////////////////////
/// constructor&destructor
////////////////////////////////////////////////////////////////////////////////
WavSelector::WavSelector(){
  
}

WavSelector::WavSelector(const string pathIn, const string sep,
                         const string comm, const bool normalize_after){
  if (pathIn.empty()){
    throw invalid_argument("WavSelector: empty path to constructor not allowed");
  }
  separator = sep;
  commentMarker = comm;
  unifDist = new uniform_real_distribution<double>(0.0, 1.0);
  if (!pathIn.empty()){
    ifstream inList(pathIn.c_str());
    if(inList.is_open()==false){
      throw invalid_argument("WavSelector: Unable to open input file: "+pathIn);
    }
    string s; // temporary buffer holding infile lines of text
    while(getline(inList, s)){
      if(s.compare(0, comm.length(), comm)==0){continue;} // ignore comment lines
      int sepPos = s.find_first_of(sep);
      const double weight = stoi(s.substr(0, sepPos));
      const string wavpath = s.substr(sepPos+1);
      add(wavpath, weight);
    }
    inList.close();
    if(normalize_after){normalize();}
  }
}

WavSelector::WavSelector(const vector<string> filenames,
                         const bool normalize_after){
  separator = "|";
  commentMarker = "#";
  unifDist = new uniform_real_distribution<double>(0.0, 1.0);
  for(string s : filenames){
    add(s, 1);
  }
  if(normalize_after){normalize();}
}

WavSelector::~WavSelector(){
  delete unifDist;
}


////////////////////////////////////////////////////////////////////////////////
/// getters
////////////////////////////////////////////////////////////////////////////////
map<string, double> WavSelector::getContent(){
  return content;
}

////////////////////////////////////////////////////////////////////////////////
/// further functionality
////////////////////////////////////////////////////////////////////////////////
double WavSelector::totalWeights() const{
  double total = 0;
  for (auto x : content){total += x.second;}
  return total;
}

void WavSelector::normalize(){
  double t = totalWeights();
  if(t<=0){cout << "WavSelector.normalize: malformed data! Aborting." << endl;}
  else{for (auto x : content){
      content[x.first] /= t;
    }
  }
}

string WavSelector::pick(){
  double r = (*unifDist)(randomEngine)*totalWeights();
  double accum = 0;
  for (auto x : content){
    accum += x.second;
    if(accum>=r){
      return x.first;
    }
  }
  throw invalid_argument("WavSelector.pick(): this should never happen!");
}

string WavSelector::asString() const{
  ostringstream oss;
  oss << commentMarker << " Content: <NORMALIZED_WEIGHT> " << separator <<
    " <PATH_TO_WAV_FILE>" << endl << commentMarker <<
    " fields are separated by '" << separator << "'" << endl << commentMarker <<
    " Lines starting with '" <<  commentMarker <<
    "' (comments) will be ignored" << endl;
  for (auto x : content){
      oss << x.first  << separator<< x.second << endl;
    }
  return oss.str();
}

void WavSelector::add(const string wavPath, const double weight,
                      const bool normalize_after){
  // try to insert new entry. If preexistent, just add to entry
  if (content.insert(make_pair(wavPath, weight)).second==false){
    content[wavPath] += weight;
  }
  if(content[wavPath]<0){content[wavPath]=0;} // no negative dist. allowed!
  if(normalize_after){normalize();}
}

void WavSelector::toFile(const string pathOut){
  ofstream outFile(pathOut, ios::out);
  if(!outFile.is_open()){
    cout << "WavSelector: couldn't export to " << pathOut << ". Aborting"<<endl;
    return;
  } else{ // if able to open file
    cout << "WavSelector: writing to " << pathOut << endl;
    outFile << asString();
    outFile.close();
    cout << "wrote "<< pathOut << " succesfully!" << endl;
  }
}

