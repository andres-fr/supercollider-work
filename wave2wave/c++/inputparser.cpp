// stl includes
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
// own header
#include "inputparser.h"
// namespace
using namespace std;

InputParser::InputParser (int &argc, char **argv, string workspaceDir)
  : action(""), originalPath(""), sampledownRatio(0), iterations(0),
    projectPath(""), dListName(""){
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
        dListName = token;
      } else if (!current_flag.compare("-p")){
        projectPath = workspaceDir+token + "/";
      } else {
        cout << "InputParser: malformed argument list " << current_flag << endl;
      }
    }
  }
  //prettyPrint();
}

void InputParser::prettyPrint(){
  cout << "action=" << action << endl;
  cout << "originalPath=" << originalPath << endl;
  cout << "materialPaths=(" << endl;
  for (auto i = materialPaths.begin(); i != materialPaths.end(); ++i){
    cout << "     " << *i << endl;
  }
  cout << ")" << endl;
  cout << "sampledownRatio=" << sampledownRatio << endl;
  cout << "iterations=" << iterations << endl;
  cout << "projectPath=" << projectPath << endl;
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

string InputParser::getDListName() const{
  return dListName;
}

string InputParser::getProjectPath() const{
  return projectPath;
}
