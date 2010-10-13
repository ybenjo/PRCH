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

double TOT::get_uniform_rand(){
  return (double)rand() * (1.0 / (RAND_MAX + 1.0));
}

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

void TOT::set_PSI_and_BETA(){
  //PSI及びBETAを計算する
  PSI.clear();
  BETA.clear();
  for(unint i = 0; i < K; ++i){
    vector<double> each_psi, m_and_v = calc_mean_and_var(T_Z[i]);
    double mean = m_and_v[0], var = m_and_v[1];
    double tmp = (mean * (1-mean)/var - 1);
    each_psi.push_back(mean * tmp);
    each_psi.push_back((1-mean) * tmp);
    PSI.push_back(each_psi);
    BETA.push_back(calc_BETA(each_psi[0], each_psi[1]));
  }

  //計算が終わったらtopic-timeのデータであるT_Zを初期化する
  T_Z.clear();
}

//tgammaは負数を受け付けないので、しょうがないから実装する
//負じゃない場合はtgammaをそのまま返す
double TOT::gamma(double x){
  if(x > 0){
    return tgamma(x);
  }else{
    double xxx = -1 * x + 1;
    return M_PI / (sin(M_PI * xxx) * tgamma(xxx));
  }
}

//ベータ関数
double TOT::calc_BETA(double x, double y){
  double ret = gamma(x) * gamma(y) / gamma(x + y);
  //val != valならばvalがnanなので0を返す
  if(ret != ret){
    return 0;
  }else{
    return ret;
  }
}
