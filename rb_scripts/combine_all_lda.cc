#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

vector<string> split(string line){
  vector<string> words;
  stringstream ss(line);
  string elem;
  while (ss >> elem){
    words.push_back(elem);
  }
  return words;
}


int main(int argc, char **argv){
  char *year = argv[1];
  ifstream ifs;
  string line;
  string path = "../patent/lda_data/";

  map<pair<string, string>, int> total;

  ostringstream oss;
  oss << path << year << "_all_all_document_prev" << ".txt";
  ifs.open((oss.str()).c_str(), ios::in);
  cout << "Reading " << (oss.str()).c_str() << endl;
  while(getline(ifs, line)){
    vector<string> elem = split(line);
    string doc_id = elem[0];
    string word_id = elem[1];
    int count = atoi(elem[2].c_str());
    total[make_pair(doc_id, word_id)] += count;
  }
  ifs.close();

  ofstream ofs;
  ostringstream oss_out;
  oss_out << path << year << "_all_all_document.txt";
  ofs.open((oss_out.str()).c_str());
  cout << "Writing " << (oss_out.str()).c_str() << endl;
  map<pair<string, string>, int>::iterator i;
  for(i = total.begin(); i != total.end(); ++i){
    string doc_id = (i->first).first;
    string word_id = (i->first).second;
    int count = i->second;
    ofs << doc_id << "\t" << word_id << "\t" << count << endl;
   }
  ofs.close();
}
