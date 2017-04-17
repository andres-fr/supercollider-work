#ifndef WAVSEQUENCE_H
#define WAVSEQUENCE_H

// stl includes
#include <vector>
// other libraries

// namespaces
using namespace std;


struct wavsequence_entry{int delay; double k; string wavPath;};

class WavSequence {
 private:
  vector<wavsequence_entry> content;
  string separator;
  string commentMarker;
 public:
  // constructors
  explicit WavSequence(const string pathIn, const string sep,
                       const string comm);
  explicit WavSequence(const string sep, const string comm);
  // deleted copy- and assignment constructors
  WavSequence(const WavSequence& copyFrom)=delete;
  WavSequence& operator= (const WavSequence& copyFrom)=delete;
  // destructor
  ~WavSequence();
  // getters
  vector<wavsequence_entry> getContent();
  // further functionality
  string asString() const;
  void add(const int delay, const double k, const string wavPath);
  void toFile(const string pathOut);
};

#endif
