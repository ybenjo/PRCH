#include "./lda.hpp"
#include <unistd.h>

int main(int argc, char **argv){
  char *input_filename = NULL, *output_filename = NULL, *dic_filename = NULL;
  bool debug_flag = false;
  unint result, k = 10, iteration = 10, limit = 10;
  double alpha = 0, beta = 0.1;

  while(1){
    result = getopt(argc, argv, "i:o:a:k:t:b:l:d:x");
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
    case 'x' : debug_flag = true; break;
    }
    optarg = NULL; 
  }

  //初期値として50/kを代入
  if(alpha == 0){
    alpha = 50 / k;
  }

  if(output_filename == NULL){
    output_filename = "./result";
  }

  if(input_filename == NULL){
    cout << "No input file!" << endl;
    exit(1);
  }

  
  LDA lda(alpha, beta, k, debug_flag);
  lda.read_file(input_filename);
  lda.all_sampling(iteration);
  lda.read_dictionary(dic_filename);
  lda.output(output_filename, limit, dic_filename);
}
