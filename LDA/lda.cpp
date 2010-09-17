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

  //Nを初期化
  this->set_initial_N_all();
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
    this->set_Z(doc_id, word_id, i, topic);
    this->N_kj[key(topic, doc_id)]++;
    this->N_wk[key(word_id, topic)]++;
    this->N_k[topic]++;
  }
}

void LDA::set_Z(unint doc_id, unint word_id, unint t, unint topic){
  this->Z[make_pair(key(doc_id, word_id), t)] = topic;
}

unint LDA::get_Z(unint doc_id, unint word_id, unint t){
  return this->Z[make_pair(key(doc_id, word_id), t)];
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
    prob[i] = calc_prob(doc_id, word_id, i);
    if(i != 0){
      prob[i] += prob[i-1];
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

//tはdoc_idにおいてword_idのうち何番目かを示す
void LDA::sampling(unint doc_id, unint word_id, unint t){
  unint topic = this->get_Z(doc_id, word_id,  t);
  //まずは一度取り除く
  this->N_kj[key(topic, doc_id)]--;
  this->N_wk[key(word_id, topic)]--;
  this->N_k[topic]--;

  //サンプリングにより新たなトピックを選ぶ
  unint new_topic = select_new_topic(doc_id, word_id);
  
  //N_*な変数を更新
  this->set_Z(doc_id, word_id, t, new_topic);
  this->N_kj[key(new_topic, doc_id)]++;
  this->N_wk[key(word_id, new_topic)]++;
  this->N_k[new_topic]++;
}

void LDA::all_sampling(unint count){
  map<key, unint>::iterator i;
  for(int j = 0;j < count;++j){
    cout << j + 1 << "'s iteration" << endl;
    for(i = this->bag_of_words.begin(); i != this->bag_of_words.end();++i){
      unint doc = (i->first).first;
      unint word = (i->first).second;
      unint t = i->second;
      this->sampling(doc, word, t);
    }
  }
}

void LDA::output(const char *filename, unint limit){
  map<key, unint>::iterator i;
  for(i = this->N_kj.begin(); i != this->N_kj.end(); ++i){
    unint j = (i->first).second;
    this->N_j[j] += i->second;
  }
  
  map<key, double> phi, theta;
    
  //phiの計算
  for(i = this->N_wk.begin(); i != this->N_wk.end(); ++i){
    unint word = (i->first).first;
    unint topic = (i->first).second;
    phi[key(word, topic)] = (this->N_wk[key(word, topic)] + this->beta) / (this->N_k[topic] + this->W * this->beta);
  }
  
  //thetaの計算
  //   for(i = this->N_kj.begin(); i != this->N_kj.end(); ++i){
  //     unint topic = (i->first).first;
  //     unint doc = (i->first).second;
  //     theta[key(topic, doc)] = (this->N_kj[key(topic, doc)] + this->alpha) / (this->N_j[doc] + this->K * this->alpha);
  //   }

  //トピックごとに単語の上位limit件を出力
  ofstream ofs;
  ofs.open(filename);
  for(int k = 0; k < this->K; ++k){
    multimap<double, unint> phi_;

    //multimapに入れて確率順にソート
    for(i = this->N_wk.begin(); i != this->N_wk.end(); ++i){
      unint topic = (i->first).second;
      if(topic == k){
	unint word = (i->first).first;
	phi_.insert(make_pair(phi[key(word, topic)], word));
      }
    }
    
    //出力
    multimap<double, unint>::reverse_iterator rev;
    ofs << "# " <<  k << "'s topic" << endl;
    int count = 0;
    for(rev = phi_.rbegin(); rev != phi_.rend(); ++rev){
      count++;
      ofs << rev->second << "," << rev->first << endl;
      if(count > limit){break;}
    }
  }
  ofs.close();
}

