include "tot.hpp"

void TOT::read_file(char *file_name){

  if(filename == NULL){
    cout << "No input file!" << endl;
    exit(1);
  }
  
  ifstream ifs;
  input_file_name = filename;
  ifs.open(filename, ios::in);
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
