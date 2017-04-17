#ifndef WAVSELECTOR_H
#define WAVSELECTOR_H

// stl includes
#include <map>
// other libraries

// namespaces
using namespace std;


class WavSelector {
 private:
  default_random_engine randomEngine;
  uniform_real_distribution<double>* unifDist;
  map<string, double> content;
  string separator;
  string commentMarker;
 public:
  // constructors
  explicit WavSelector(const string pathIn, const string sep,
                       const string comm, const bool normalize_after=false);
  explicit WavSelector(const string sep, const string comm);
  // deleted copy- and assignment constructors
  WavSelector(const WavSelector& copyFrom)=delete;
  WavSelector& operator= (const WavSelector& copyFrom)=delete;
  // destructor
  ~WavSelector();
  // getters
  map<string, double> getContent();
  // further functionality
  double totalWeights() const;
  void normalize(); // side effects!
  string pick();
  string asString() const;
  void add(const string wavPath, const double weight,
           const bool normalize_after=false);
  void toFile(const string pathOut);
};

#endif
