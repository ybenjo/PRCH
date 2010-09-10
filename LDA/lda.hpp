#ifndef __class__LDA__
#define __class__LDA__

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

using namespace std;

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
  
  void read_file(char *filename);
  vector<string> split(string line);
  void set_bag_of_words(unint doc_id, unint word_id, unint count);
  
  double get_uniform_rand();
  void set_N();
protected:
  //サンプリング時のパラメータ
  double alpha, beta;
  //Kはトピック数(パラメータ), Wはwordのユニーク数
  unint K, W;
  //keyはdoc_id, valueはword_idが入ったvector
  map<unint, vector<unint> > bag_of_words;
  //
  map<key, int> N_kj, N_wk;
  //word_idのuniqを取るために使うset
  set<unint> uniq_word_id;
};


#endif //__class__LDA__
