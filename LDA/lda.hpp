#ifndef __class__LDA__
#define __class__LDA__

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace std;

//
typedef unsigned int unint;
typedef pair<unint, unint> key;
typedef pair<key, unint> triple_key;

class LDA{
public:
  LDA(){
    srand(time(0));
  };
  
  LDA(double alpha, double beta){
    this->alpha = alpha;
    this->beta = beta;
    srand(time(0));
  };
  
  void read_file(char & filename);
  double get_uniform_rand();
  void set_N();
protected:
  double alpha, beta;
  unint K, W;
  map<key, unint> N_kj, N_wk;
};


#endif //__class__LDA__
