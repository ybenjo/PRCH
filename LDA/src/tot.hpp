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
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

typedef int unint;
typedef std::pair<unint, unint> key;

class TOT{
public:
  TOT(){
    srand(time(0));
  };
  TOT(double alpha, double beta, unint topic, bool debug_flag = false){
    this->alpha = alpha;
    this->beta = beta;
    this->K = topic;
    this->debug_flag = debug_flag;
    srand(time(0));
  };

  //logger
  template <typename T_n> void show_debug(T_n message){
    if(debug_flag){
      cout << message << endl;
    }
  }

  void read_file(char *file_name);
  vector<string> split(string line);

  //setter
  void set_N_and_Z(unint doc_id, unint doc_time, unint word_id, unint count);
  void set_PSI_and_BETA();

  //getter
  vector<vector<unint> > get_Z(){return Z;};
  vector<vector<double> > get_PSI(){return PSI;};

  //サンプリング周りの関数
  double get_uniform_rand();
  vector<double> calc_mean_and_var(const vector<unint>& years);
  double calc_BETA(double x, double y);
  double gamma(double x);

private:
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

  //PSIはベータ関数などに用いるパラメータ
  //<psi_1, psi_2>をvalueに持つvector。topicにはインデックスでアクセス
  vector<vector<double> > PSI;

  //ベータ関数の値を持つvector
  vector<double> BETA;

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
