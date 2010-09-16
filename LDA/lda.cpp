#include "lda.hpp"

void LDA::read_file(char *filename){
  ifstream ifs;
  ifs.open(filename, ios::in);
  string line;
  
  while(getline(ifs, line)){
    //想定するファイルの形式は
    //doc_id \t word_id \t count
    vector<string> elem = this->split(line);
    unint doc_id = atoi(elem[0].c_str());
    unint word_id = atoi(elem[1].c_str());
    unint count = atoi(elem[2].c_str());
    this->set_bag_of_words(doc_id, word_id, count);
  }
  ifs.close();
  
  //ユニークな単語数を取得
  this->W = this->uniq_word_id.size();
}

vector<string> LDA::split(string line){
  vector<string> words;
  stringstream ss(line);
  string elem;
  while (ss >> elem){
    words.push_back(elem);
  }
  return words;
}

void LDA::set_bag_of_words(unint doc_id, unint word_id, unint count){
  this->bag_of_words[key(doc_id, word_id)] = count;
  this->uniq_word_id.insert(word_id);
}

void LDA::set_initial_N(unint doc_id, unint word_id){
  unint count = this->bag_of_words[key(doc_id, word_id)];
  for(int i = 0; i < count; ++i){
    //初期のtopicはランダムに割り振る
    unint topic = rand() / (RAND_MAX / (this->K));
    Z[key(doc_id, word_id)] = topic;
    N_kj[key(topic, doc_id)]++;
    N_wk[key(word_id, topic)]++;
    N_k[topic]++;
  }
}

void LDA::set_initial_N_all(){
  map<key, unint>::iterator i;
  for(i = this->bag_of_words.begin(); i != this->bag_of_words.end(); ++i){
    this->set_initial_N((i->first).first, (i->first).second);
  }
}

double LDA::get_uniform_rand(){
  return (double)rand() * (1.0 / (RAND_MAX + 1.0));
}

double LDA::calc_prob(unint doc_id, unint word_id, unint topic){
  //この関数を呼ぶ時点で、N_**の変数からdocとwordに対応する値が
  //デクリメントされている事が前提
  return (this->N_kj[key(topic, doc_id)] + this->alpha) *
    (this->N_wk[key(word_id, topic)] + this->beta) /
    (this->N_k[topic] + this->W * this->beta);
}

unint LDA::select_new_topic(unint doc_id, unint word_id){
  //サンプリング用の一様乱数を生成
  double u = get_uniform_rand();
  
  //サンプリング用の確率分布を生成する
  map<unint, double> prob;
  for(int i = 0; i < this->K; ++i){
    if(i == 0){
      prob[i] = calc_prob(doc_id, word_id, i);
    }else{
      prob[i] = prob[i-1] + calc_prob(doc_id, word_id, i);
    }
  }

  //サンプリング
  unint new_topic = 0;
  for(int k = 0; k < this->K; ++k){
    if(u < prob[k] / prob[this->K - 1]){
      new_topic = k;
      break;
    }
  }
  return new_topic;
}

void LDA::sampling(unint doc_id, unint word_id){
  unint topic = this->Z[key(doc_id, word_id)];
  //まずは一度取り除く
  this->N_kj[key(topic, doc_id)]--;
  this->N_wk[key(word_id, topic)]--;
  this->N_k[topic]--;

  //サンプリングにより新たなトピックを選ぶ
  unint new_topic = select_new_topic(doc_id, word_id);
  
  //N_*な変数を更新
  this->Z[key(doc_id, word_id)] = new_topic;
  this->N_kj[key(new_topic, doc_id)]++;
  this->N_wk[key(word_id, new_topic)]++;
  this->N_k[new_topic]++;
}
