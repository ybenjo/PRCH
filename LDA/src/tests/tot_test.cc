#include <gtest/gtest.h>
#include "../tot.hpp"

TEST(tot, read_file){
  TOT sampler(1.0, 0.1, 1);
  sampler.read_file("./src/tests/tot_test.txt");
  // 1       2000	1       1
  // 1       2000	2       2
  // 1       2000	3       3
  // 2       1995	1       2
  // 2       1995	4       3
  vector<vector<unint> > Z = sampler.get_Z();
  //1 2000 1 * 
  EXPECT_EQ(1, Z[0][0]);
  EXPECT_EQ(2000, Z[0][1]);
  EXPECT_EQ(1, Z[0][2]);
  EXPECT_EQ(0, Z[0][3]);
  //1 2000 2 * 
  EXPECT_EQ(1, Z[1][0]);
  EXPECT_EQ(2000, Z[1][1]);
  EXPECT_EQ(2, Z[1][2]);
  EXPECT_EQ(0, Z[1][3]);
  EXPECT_EQ(0, Z[1][4]);
  //1 2000 3 * 
  EXPECT_EQ(1, Z[2][0]);
  EXPECT_EQ(2000, Z[2][1]);
  EXPECT_EQ(3, Z[2][2]);
  EXPECT_EQ(0, Z[2][3]);
  EXPECT_EQ(0, Z[2][4]);
  EXPECT_EQ(0, Z[2][5]);
  //2 1995 1 * 
  EXPECT_EQ(2, Z[3][0]);
  EXPECT_EQ(1995, Z[3][1]);
  EXPECT_EQ(1, Z[3][2]);
  EXPECT_EQ(0, Z[3][3]);
  EXPECT_EQ(0, Z[3][4]);
  //2 1995 1 * 
  EXPECT_EQ(2, Z[4][0]);
  EXPECT_EQ(1995, Z[4][1]);
  EXPECT_EQ(4, Z[4][2]);
  EXPECT_EQ(0, Z[4][3]);
  EXPECT_EQ(0, Z[4][4]);
  EXPECT_EQ(0, Z[4][3]);
}

int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
