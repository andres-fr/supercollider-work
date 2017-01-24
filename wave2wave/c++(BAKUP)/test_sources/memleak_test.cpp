// g++ -std=c++14 -Wall -Wextra /home/afr/git/supercollider-work/wave2wave/c++/MAIN.cpp -o MAIN && valgrind --leak-check=yes ./MAIN

#include <vector>
#include <iostream>
using namespace std;


class Leaker{
public:
  explicit Leaker(int i);
  explicit Leaker(const Leaker& l);
  Leaker& operator=(const Leaker& l);
  ~Leaker();
private:
  int* x;
};

Leaker::Leaker(int i){
  x = new int[i]();
}

Leaker::~Leaker(){
  delete[] x;
}

int main()

{

  Leaker asdf(100);
  vector<int> fdsa(100);

  vector<int>* xx = new vector<int>(1);
  delete xx;


  cout << "guadefac"<<endl;
}
