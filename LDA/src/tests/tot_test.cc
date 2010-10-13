#include <gtest/gtest.h>
#include "../tot.hpp"

TEST(tot, read_file){
  //トピック数を1にしているので全てのtokenはトピック0に割り振られなければならない
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

TEST(tot, calc_mean_and_var){
  TOT sampler(1.0, 0.1, 1);
  //test = [0,9]
  vector<unint> test;
  for(unint i = 1; i < 10; ++i){
    test.push_back(i);
  }
  vector<double> mean_and_var = sampler.calc_mean_and_var(test);
  EXPECT_DOUBLE_EQ(5, mean_and_var[0]);
  EXPECT_DOUBLE_EQ(60.0 / 9, mean_and_var[1]);
}

TEST(tot, gamma){
  TOT sampler(1.0, 0.1, 1);
  EXPECT_DOUBLE_EQ(tgamma(1.5), sampler.gamma(1.5));
  EXPECT_DOUBLE_EQ(2.363271801207355, gamma(-1.5));
}

TEST(tot, set_PSI_and_BETA){
  TOT sampler(1.0, 0.1, 1);
  sampler.read_file("./src/tests/tot_test.txt");
  sampler.set_PSI_and_BETA();
  vector<vector<double> > psi = sampler.get_PSI();
  EXPECT_DOUBLE_EQ(psi[0][0], -1285629795.0);
  EXPECT_DOUBLE_EQ(psi[0][1],  1284986248.8);
  vector<double> beta = sampler.get_BETA();
  //psi[0][0]が負の整数のため、betaがnanになる(-> 0を返す)
  EXPECT_DOUBLE_EQ(beta[0], 0);
}

int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
