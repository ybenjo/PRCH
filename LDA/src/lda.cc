#include "./lda.hpp"

void LDA::read_file(char *filename){

  if(filename == NULL){
    cout << "No input file!" << endl;
    exit(1);
  }
  
  ifstream ifs;
  this->input_file_name = filename;
  ifs.open(filename, ios::in);
  string line;

  this->show_debug("Start reading file.");
  int line_count = 0;
  while(getline(ifs, line)){
    //想定するファイルの形式は
    //doc_id \t word_id \t count
    vector<string> elem = this->split(line);
    unint doc_id = atoi(elem[0].c_str());
    unint word_id = atoi(elem[1].c_str());
    unint count = atoi(elem[2].c_str());

    //bag_of_wordsへの挿入
    this->uniq_word_id.insert(word_id);
    this->set_N_and_Z(doc_id, word_id, count);

    line_count++;
    this->show_debug(line_count);
  }
  ifs.close();

  this->show_debug("Finish reading file and set N and Z.");
  
  //ユニークな単語数を取得
  this->W = uniq_word_id.size();
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

void LDA::set_N_and_Z(unint doc_id, unint word_id, unint count){
  vector<unint> each_doc;
  each_doc.push_back(doc_id);
  each_doc.push_back(word_id);
  for(int i = 0; i < count; ++i){
    //初期のtopicはランダムに割り振る
    unint topic = rand() / (RAND_MAX / (this->K));
    each_doc.push_back(topic);
    this->N_kj[key(topic, doc_id)]++;
    this->N_wk[key(word_id, topic)]++;
    this->N_k[topic]++;
  }
  this->Z.push_back(each_doc);
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
unint LDA::sampling(unint doc_id, unint word_id, unint prev_topic){
  //まずは一度取り除く
  this->N_kj[key(prev_topic, doc_id)]--;
  this->N_wk[key(word_id, prev_topic)]--;
  this->N_k[prev_topic]--;

  //サンプリングにより新たなトピックを選ぶ
  unint new_topic = select_new_topic(doc_id, word_id);
  
  //N_*な変数を更新
  this->N_kj[key(new_topic, doc_id)]++;
  this->N_wk[key(word_id, new_topic)]++;
  this->N_k[new_topic]++;

  return new_topic;
}

void LDA::all_sampling(unint count){
  //全文書全単語についてサンプリングを行う
  for(int j = 0;j < count;++j){
    cout << j + 1 << "'s iteration" << endl;
    int z_size = Z.size();
    for(int z_index = 0; z_index < z_size ;++z_index){
      ostringstream oss;
      oss << "Now " << z_index << " / " << z_size;
      show_debug(oss.str());
      this->sampling_word(z_index);
    }
  }
}

void LDA::sampling_word(unint z_index){
  //doc_idにおけるword_idの登場回数だけサンプリングを行う
  unint doc_id = Z[z_index][0];
  unint word_id = Z[z_index][1];
  int word_count = Z[z_index].size();
  
  //<doc_id, word_id, topic_1, topic_2, ..., topic_{word_count}>なので
  //2つめ以降のみを見ていく
  for(int i = 2;i < word_count; ++i){
    unint prev_topic = Z[z_index][i];
    unint new_topic = this->sampling(doc_id, word_id, prev_topic);
    //トピックの更新
    Z[z_index][i] = new_topic;
  }
}

void LDA::read_dictionary(char *filename){
  //filenameがNULLの時は読み込みを行わない
  if(filename != NULL){
    ifstream ifs;
    ifs.open(filename, ios::in);
    string line;
    
    while(getline(ifs, line)){
      //想定する辞書ファイルの形式は
      //word_id \t word
      vector<string> elem = this->split(line);
      unint word_id = atoi(elem[0].c_str());
      string word = elem[1];
      this->dic[word_id] = word;
    }
    ifs.close();
  }
}

void LDA::output(char *filename, unint limit, char *flag){
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

  //出力ファイル名生成
  ostringstream oss;
  if(filename == NULL){
    oss << this->input_file_name << "_result_a" << alpha << "b" << beta << "k" << K << "L" << limit;
  }else{
    oss << filename;
  }

  cout << oss.str() << endl;

  //トピックごとに単語の上位limit件を出力
  ofstream ofs;
  ofs.open((oss.str()).c_str());
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
      if(flag != NULL){
	ofs << this->dic[rev->second] << "," << rev->first << endl;
      }else{
	ofs << rev->second << "," << rev->first << endl;
      }
      if(count > limit){break;}
    }
  }
  ofs.close();
}

