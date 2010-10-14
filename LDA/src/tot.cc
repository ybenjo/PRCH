#include "./tot.hpp"

void TOT::read_file(char *file_name){

  if(file_name == NULL){
    cout << "No input file!" << endl;
    exit(1);
  }
  
  ifstream ifs;
  input_file_name = file_name;
  ifs.open(file_name, ios::in);
  string line;

  show_debug("Start reading file.");
  cout << "Start reading file." << endl;
  int line_count = 0;
  while(getline(ifs, line)){
    //想定するファイルの形式は
    //doc_id \t time \t word_id \t count
    vector<string> elem = this->split(line);
    unint doc_id = atoi(elem[0].c_str());
    unint doc_time = atoi(elem[1].c_str());
    unint word_id = atoi(elem[2].c_str());
    unint count = atoi(elem[3].c_str());

    //bag_of_wordsへの挿入
    uniq_word_id.insert(word_id);
    set_N_and_Z(doc_id, doc_time, word_id, count);

    line_count++;
    show_debug(line_count);
  }
  ifs.close();

  show_debug("Finish reading file and set N and Z.");
  cout << "Finish reading file and set N and Z." << endl;
  
  //ユニークな単語数を取得
  W = uniq_word_id.size();
}

//ここでZは
//<doc_id, doc_time, word_id, topic, ..., topic>を要素にもつvector
void TOT::set_N_and_Z(unint doc_id, unint doc_time, unint word_id, unint count){
  vector<unint> each_doc;
  each_doc.push_back(doc_id);
  each_doc.push_back(doc_time);
  each_doc.push_back(word_id);
  for(int i = 0; i < count; ++i){
    //初期のtopicはランダムに割り振る
    unint topic = rand() / (RAND_MAX / (this->K));
    each_doc.push_back(topic);
    N_kj[key(topic, doc_id)]++;
    N_wk[key(word_id, topic)]++;
    N_k[topic]++;
    T_Z[topic].push_back(doc_time);
  }
  this->Z.push_back(each_doc);
}

vector<string> TOT::split(string line){
  vector<string> words;
  stringstream ss(line);
  string elem;
  while (ss >> elem){
    words.push_back(elem);
  }
  return words;
}

//0から1の一様乱数生成
double TOT::get_uniform_rand(){
  return (double)rand() * (1.0 / (RAND_MAX + 1.0));
}

//vectorから平均と「標本」分散を計算しvectorとして返す
vector<double> TOT::calc_mean_and_var(const vector<unint>& years){
  vector<unint>::const_iterator i;
  unint size = years.size();
  double mean = 0.0, var = 0.0;
  vector<double> ret;
  
  for(i = years.begin(); i != years.end(); ++i){
    mean += *i;
  }
  mean /= size;
  ret.push_back(mean);
    
  for(i = years.begin(); i != years.end(); ++i){
    var += pow((*i - mean), 2);
  }
  var /= size;
  ret.push_back(var);

  return ret;
}

//パラメータ対PSI及びベータ関数の値BETAを計算する
void TOT::set_PSI_and_BETA(){
  //PSI及びBETAを初期化
  PSI.clear();
  BETA.clear();
  for(unint i = 0; i < K; ++i){
    vector<double> each_psi, m_and_v = calc_mean_and_var(T_Z[i]);
    double mean = m_and_v[0], var = m_and_v[1];
    //元論文10ページ右下の式を実装
    double tmp = (mean * (1-mean)/var - 1);
    each_psi.push_back(mean * tmp);
    each_psi.push_back((1-mean) * tmp);
    PSI.push_back(each_psi);
    BETA.push_back( calc_BETA(each_psi[0], each_psi[1]) );
  }

  //計算が終わったらtopic-timeのデータであるT_Zを初期化する
  T_Z.clear();
}

//tgammaは負数を受け付けないので、しょうがないから実装する
//負じゃない場合はtgammaをそのまま返し、負の場合整数かどうかの判定は行わずにそのまま返す。
//その時の式はman tgammaのものを使用
double TOT::gamma(double x){
  if(x > 0){
    return tgamma(x);
  }else{
    double xxx = -1 * x + 1;
    return M_PI / ( sin(M_PI * xxx) * tgamma(xxx) );
  }
}

//ベータ関数
//負の整数によってnanになった場合は0を返す
double TOT::calc_BETA(double x, double y){
  double ret = gamma(x) * gamma(y) / gamma(x + y);
  //val != valならばvalがnanなので0を返す
  if(ret != ret){
    return 0;
  }else{
    return ret;
  }
}

//元論文3ページ右下の式を実装
double TOT::calc_prob(unint doc_id, unint doc_time, unint word_id, unint topic){
  //BETA[topic]が0である場合には0除算エラーが起こるので0を返す
  if(BETA[topic] == 0){
    return 0;
  }else{
    return ( N_kj[key(topic, doc_id)] + alpha - 1 ) *
      ( N_wk[key(word_id, topic)] + beta - 1 ) / ( N_k[topic] + W * beta - 1 ) *
      ( pow(1 - doc_time, PSI[topic][0] -1) * pow(doc_time, PSI[topic][1] - 1) ) / BETA[topic];
  }
}

//new_topicを返す関数
unint TOT::select_new_topic(unint doc_id, unint doc_time, unint word_id){
  //サンプリング用の一様乱数を生成
  double u = get_uniform_rand();

  //サンプリング用の確率分布を生成する
  map<unint, double> prob;
  for(int i = 0; i < K; ++i){
    prob[i] = calc_prob(doc_id, doc_time, word_id, i);
    if(i != 0){
      prob[i] += prob[i-1];
    }
  }

  //uは0-1のため、probの最大値をかけて0-Kの範囲に引き伸ばす
  u *= K;

  //サンプリング
  unint new_topic = 0;
  for(int k = 0; k < K; ++k){
    if(u < prob[k] / prob[K - 1]){
      new_topic = k;
      break;
    }
  }
  return new_topic;
}

//Zの1要素(word単位)に対するサンプリングを行う
//なのであるtokenの登場回数だけループを回す事になる
void TOT::sampling_word(unint z_index){
  unint doc_id = Z[z_index][0];
  unint doc_time = Z[z_index][1];
  unint word_id = Z[z_index][2];
  unint z_size = Z[z_index].size();
  
  //Z[z_index] = [doc_id, doc_time, word_id, topic1, ..., topic_{count}]なので3つ目以降を見ていく  
  for(unint token_index = 3;token_index < z_size; ++token_index){
    unint prev_topic = Z[z_index][token_index];
    unint new_topic = select_new_topic(doc_id, doc_time, word_id);
    //new_topicが決まったのでN_**やT_Zを更新する
    N_kj[key(prev_topic, doc_id)]--;
    N_wk[key(word_id, prev_topic)]--;
    N_k[prev_topic]--;
  
    N_kj[key(new_topic, doc_id)]++;
    N_wk[key(word_id, new_topic)]++;
    N_k[new_topic]++;

    T_Z[new_topic].push_back(doc_time);
  }
}

void TOT::sampling_all_Z(){
  unint z_size = Z.size();
  for(unint z_index = 0; z_index < z_size; ++z_index){
    ostringstream oss;
    oss << "Now " << z_index + 1 << " / " << z_size;
    show_debug(oss.str());
    sampling_word(z_index);
  }
}

void TOT::sampling(unint count){
  for(unint i = 0; i < count; ++i){
    ostringstream oss;
    oss << i + 1  << "'s iteration";
    show_debug(oss.str());
    cout << i + 1  << "'s iteration" << endl;
    
    sampling_all_Z();
    set_PSI_and_BETA();
  }
}
void TOT::read_dictionary(char *filename){
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

void TOT::output(char *filename, unint limit, char *flag){
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

