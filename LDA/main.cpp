#include "./lda.hpp"
#include <unistd.h>

int main(int argc, char **argv){
  char *input_filename, *output_filename;
  unint result, k, iteration, limit;
  double alpha, beta = 0.1;

  while(1){
    result = getopt(argc, argv, "i:o:a:k:t:b:l:");
    if(result == -1) break;
    
    switch(result){
    case 'i' : input_filename = optarg; break; 
    case 'o' : output_filename = optarg; break;
    case 'a' : alpha = atof(optarg); break;
    case 'b' : beta = atof(optarg); break;
    case 'k' : k = atoi(optarg); break;
    case 't' : iteration = atoi(optarg); break;
    case 'l' : limit = atoi(optarg); break;
    }
    optarg = NULL; 
  }

  LDA lda(alpha, beta, k);
  lda.read_file(input_filename);
  lda.all_sampling(iteration);
  lda.output(output_filename, limit);
}
