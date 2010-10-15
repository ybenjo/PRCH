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
  char *fname = argv[1];
  ifstream ifs;
  string line;

  map<pair<string, string>, int> total;

  ostringstream oss;
  oss << fname ;
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

  ostringstream oss_read_date;
  ifstream ifs_date;
  oss << fname << ".date" ;

  map<string, string> doc_date;
  ifs_date.open((oss_read_date.str()).c_str(), ios::in);
  cout << "Reading " << (oss_read_date.str()).c_str() << endl;
  while(getline(ifs_date, line)){
    vector<string> elem = split(line);
    string doc_id = elem[0];
    string date = elem[1];
    cout << date << endl;
    doc_date[doc_id] = date;
  }
  ifs_date.close();
  
  ofstream ofs, ofs_date;
  ostringstream oss_out, oss_date;
  
  oss_out << fname << ".txt";
  oss_date << fname << "_with_date.txt";
  ofs.open((oss_out.str()).c_str());
  ofs_date.open((oss_date.str()).c_str());
  
  cout << "Writing " << (oss_out.str()).c_str() << endl;
  map<pair<string, string>, int>::iterator i;
  for(i = total.begin(); i != total.end(); ++i){
    string doc_id = (i->first).first;
    string word_id = (i->first).second;
    int count = i->second;
    ofs << doc_id << "\t" << word_id << "\t" << count << endl;
    ofs_date << doc_id << "\t" << doc_date[doc_id] << "\t" << word_id << "\t" << count << endl;
   }
  ofs.close();
  ofs_date.close();
}
