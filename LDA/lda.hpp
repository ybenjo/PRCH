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

class LDA{
public:
  LDA(){
    srand(time(0));
  };
  
  LDA(double alpha, double beta, unint topic){
    this->alpha = alpha;
    this->beta = beta;
    this->K = topic;
    srand(time(0));
  };

  void read_file(char *filename);
  vector<string> split(string line);

  //サンプリング周りの関数
  double get_uniform_rand();
  double calc_prob(unint doc_id, unint word_id, unint topic);
  unint select_new_topic(unint doc_id, unint word_id);
  void sampling(unint doc_id, unint word_id, unint t);
  void all_sampling(unint count);
  
  //setter
  void set_bag_of_words(unint doc_id, unint word_id, unint count);
  void set_initial_N(unint doc_id, unint word_id);
  void set_initial_N_all();
  void set_Z(unint doc_id, unint word_id, unint t, unint topic);
  unint get_Z(unint doc_id, unint word_id, unint t);

  //output
  void read_dictionary(char *filename);
  void output(char *filename, unint limit, char *flag);
  
  //サンプリング時のパラメータ
  double alpha, beta;
  //Kはトピック数(パラメータ), Wはユニークな単語数
  unint K, W;
  //map<(doc_id, word_id), count>
  map<key, unint> bag_of_words;
  //N_kj -> map<(topic, doc_id), count>
  //N_wk -> map<(word_id, topic), count>
  map<key, unint> N_kj, N_wk;
  //N_kはトピックごとの単語数
  map<unint, unint> N_k;
  //N_jは文書ごとの単語数
  map<unint, unint> N_j;
  //Zはdoc_idとword_id, 登場時の順番からtopicを紐付ける変数
  map<pair<key, unint>, unint> Z;

  //dicはidとwordを結びつけるmap
  map<unint, string> dic;
  
  //word_idのuniqを取るために使うset
  set<unint> uniq_word_id;
};


#endif //__class__LDA__
