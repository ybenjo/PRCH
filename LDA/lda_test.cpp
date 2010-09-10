#include <gtest/gtest.h>
#include <lda.hpp>

TEST(lda, Testsplit){
  LDA agent;
  vector<string> ret = agent.split("これは	テスト	です");
  EXPECT_EQ("これは", ret[0]);
  EXPECT_EQ("テスト", ret[1]);
  EXPECT_EQ("です", ret[2]);
}

TEST(lda, Testread_file){
  LDA agent;
  agent.read_file("../patent/test.txt");
}


int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

