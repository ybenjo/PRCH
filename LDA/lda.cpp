#include "lda.hpp"

void LDA::read_file(char *filename){
  ifstream ifs;
  ifs.open(filename, ios::in);
  string line;
  
  while(getline(ifs, line)){
    //想定するファイルの形式は
    //doc_name \t word_name \t count
    vector<string> elem = this->split(line);
    unint doc_id = atoi(elem[0].c_str());
    unint word_id = atoi(elem[1].c_str());
    unint count = atoi(elem[2].c_str());
    this->set_bag_of_words(doc_id, word_id, count);
  }
  ifs.close();
  
  //word数を取得
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
  for(int i = 0;i < count;++i){
    this->bag_of_words[doc_id].push_back(word_id);
    this->uniq_word_id.insert(word_id);
  }
}

void LDA::set_N(){
}

double LDA::get_uniform_rand(){
  return (double)rand() * (1.0 / (RAND_MAX + 1.0));
}
