#include "./lda.hpp"
#include "./tot.hpp"
#include <unistd.h>

int main(int argc, char **argv){
  char *input_filename = NULL, *output_filename = NULL, *dic_filename = NULL;
  bool debug_flag = false;
  unint result, k = 10, iteration = 10, limit = 10, mode = NULL;
  double alpha = 0, beta = 0.1;

  while(1){
    result = getopt(argc, argv, "i:o:a:k:t:b:l:d:m:v");
    if(result == -1) break;
    
    switch(result){
    case 'i' : input_filename = optarg; break; 
    case 'o' : output_filename = optarg; break;
    case 'd' : dic_filename = optarg; break;
    case 'a' : alpha = atof(optarg); break;
    case 'b' : beta = atof(optarg); break;
    case 'k' : k = atoi(optarg); break;
    case 't' : iteration = atoi(optarg); break;
    case 'l' : limit = atoi(optarg); break;
    case 'v' : debug_flag = true; break;
    case 'm' : mode = atoi(optarg); break;
    }
    optarg = NULL; 
  }

  //初期値として50/kを代入
  if(alpha == 0){
    alpha = 50 / k;
  }

  switch(mode){
  case 0 :{
    //通常のLDAを行う
    cout << "-m 0 => normal LDA" << endl;
    LDA lda(alpha, beta, k, debug_flag);
    lda.read_file(input_filename);
    lda.all_sampling(iteration);
    lda.read_dictionary(dic_filename);
    lda.output(output_filename, limit, dic_filename);
    break;
  }
  case 1 :{
    cout << "-m 1 => Topics Over Time" << endl;
    //TOTを行う
    break;
  }
  default :{
    cout << "No  mode(-m " << mode << ")" << endl;
    exit(1);
  }
  }
}
