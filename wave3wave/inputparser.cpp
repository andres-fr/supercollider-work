// stl includes
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
// own header
#include "inputparser.h"
// namespace
using namespace std;

InputParser::InputParser (int &argc, char **argv)
  : action(""), originalPath(""), sampledownRatio(0), iterations(0),
    optListName(""), pickListName(""){
  string current_flag;
  for (int i=1; i < argc; ++i){
    string token = string(argv[i]);
    if (token[0]=='-'){
      current_flag = token;
      //cout << "flag changed to " << token << endl;
    } else{
      if (!current_flag.compare("-a")){
        action = token;
      } else if (!current_flag.compare("-s")){
        originalPath = token;
      } else if (!current_flag.compare("-m")){
        materialPaths.push_back(token);
      } else if (!current_flag.compare("-r")){
        sampledownRatio = stoi(token);
      } else if (!current_flag.compare("-i")){
        iterations = stoi(token);
      } else if (!current_flag.compare("-d")){
        optListName = token;
      } else if (!current_flag.compare("-p")){
        pickListName = token;
      } else {
        cout << "InputParser: malformed argument list " << current_flag << endl;
      }
    }
  }
}


////////////////////////////////////////////////////////////////////////////////
/// GETTERS
////////////////////////////////////////////////////////////////////////////////

string InputParser::getAction() const{
  return action;
}

string InputParser::getOriginalPath() const{
  return originalPath;
}
vector<string> InputParser::getMaterialPaths() const{
  return materialPaths;
}
unsigned int InputParser::getSampledownRatio() const{
  return sampledownRatio;
}
unsigned int InputParser::getIterations() const{
  return iterations;
}

string InputParser::getOptListName() const{
  return optListName;
}

string InputParser::getPickListName() const{
  return pickListName;
}
