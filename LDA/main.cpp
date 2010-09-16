#include "./lda.h"
#include <unistd.h>

int main(int argc, char **argv){
  char *input_filename, *output_filename;
  unint result, k, iteration;
  double alpha, beta;

  while(1){
    result = getopt(argc, argv, "i:o:a:b:k:t:");
    if(result == -1) break;
    
    switch(result){
    case 'i' : input_filename = optarg; break; 
    case 'o' : output_filename = optarg; break;
    case 'a' : entropy = atof(optarg); break;
    case 'b' : entropy = atof(optarg); break;
    case 'k' : entropy = atoi(optarg); break;
    case 't' : iteration = atoi(optarg); break;
    }
    optarg = NULL; 
  }
}
