#ifndef INPUTPARSER_H
#define INPUTPARSER_H

// stl includes
#include <vector>
// namespace
using namespace std;

class InputParser{
 private:
  string action;
  string originalPath;
  vector<string> materialPaths;
  unsigned int sampledownRatio;
  unsigned int iterations;
  string projectPath;
  string dListName;
 public:
  // constructor (parser-setter)
  InputParser(int &argc, char **argv, string workspaceDir);
  // getters
  string getAction() const;
  string getOriginalPath() const;
  vector<string> getMaterialPaths() const;
  unsigned int getSampledownRatio() const;
  unsigned int getIterations() const;
  string getDListName() const;
  string getProjectPath() const;


  //
  void prettyPrint();
};


#endif
