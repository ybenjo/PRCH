#include <gtest/gtest.h>
#include <tot.hpp>

TEST(tot, read_file){
  TOT sampler;;
  sampler.read_file("../patent/tot_test.txt");
  // 1       2000	1       1
  // 1       2000	2       2
  // 1       2000	3       3
  // 2       1995	1       2
  // 2       1995	4       3
}

int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
