#ifndef __class__TOT__
#define __class__TOT__

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

typedef int unint;
typedef std::pair<unint, unint> key;

class TOT{
public:
  TOT(double alpha, double beta, unint topic, bool debug_flag = false){
    this->alpha = alpha;
    this->beta = beta;
    this->K = topic;
    this->debug_flag = debug_flag;
    srand(time(0));
  };

  //logger
  template <typename T_n> void debug_log(T_n message){
    if(debug_flag){
      cout << message << endl;
    }
  }

  void read_file(char *filename);
  vector<string> split(string line);

    //サンプリング時のパラメータ
  double alpha, beta;
  //Kはトピック数(パラメータ), Wはユニークな単語数
  unint K, W;

  //N_kj -> map<(topic, doc_id), count>
  //N_wk -> map<(word_id, topic), count>
  map<key, unint> N_kj, N_wk;
  //N_kはトピックごとの単語数
  map<unint, unint> N_k;
  //N_jは文書ごとの単語数
  map<unint, unint> N_j;
  //Zはdoc_idとword_id, 登場時の順番からtopicを紐付ける変数
  //イメージとしては<doc_id, doc_time, word_id, topic_1, ... , topic_count>を要素にもつvector
  vector<vector<unint> > Z;

  //T_Zはtopicをkeyとして<time, ..., timeをvalueに持つmap
  map<unint, vector<unint> > T_Z;

  //PSIはベータ分布などに用いるパラメータ
  //topicをkeyとして<psi_1, psi_2>をvalueに持つmap
  map<unint, vector<unint> > PSI;

  //dicはidとwordを結びつけるmap
  map<unint, string> dic;
  
  //word_idのuniqを取るために使うset
  set<unint> uniq_word_id;

  //debug用のフラグ
  bool debug_flag;

  //output用に入力ファイル名を保持
  char *input_file_name;
};

#endif //__class__TOT__
