#include <gtest/gtest.h>
#include <lda.hpp>

TEST(lda, Testsplit){
  LDA lda;
  vector<string> ret = lda.split("これは	テスト	です");
  EXPECT_EQ("これは", ret[0]);
  EXPECT_EQ("テスト", ret[1]);
  EXPECT_EQ("です", ret[2]);
}

TEST(lda, Testread_file){
  LDA lda;
  lda.read_file("../patent/test.txt");
  // 1       1       1
  // 1       2       2
  // 1       3       3
  // 2       1       2
  // 2       4       3
  EXPECT_EQ(1, lda.bag_of_words[key(1, 1)]);
  EXPECT_EQ(2, lda.bag_of_words[key(1, 2)]);
  EXPECT_EQ(3, lda.bag_of_words[key(1, 3)]);
  EXPECT_EQ(2, lda.bag_of_words[key(2, 1)]);
  EXPECT_EQ(3, lda.bag_of_words[key(2, 4)]);
}

TEST(lda, Testset_initial_N){
  unint K = 5;
  LDA lda(0, 0, K);
  lda.read_file("../patent/test.txt");
  //こことかどうテストすればいいのか？
  cout << "1, 1 -> " << lda.get_Z(1, 1, 0) << endl;
  cout << "1, 2 -> " << lda.get_Z(1, 2, 0) << endl;
  cout << "1, 3 -> " << lda.get_Z(1, 3, 0) << endl;
  cout << "2, 1 -> " << lda.get_Z(2, 1, 0) << endl;
  cout << "2, 4 -> " << lda.get_Z(2, 4, 0) << endl;
}

TEST(lda, Testsampling){
  unint K = 5;
  LDA lda(K/50, 0.1, K);
  lda.read_file("../patent/test.txt");
  
  unint doc_id = 1, word_id = 1;
  unint init_z = lda.get_Z(doc_id, word_id, 0);
  unint init_n_kj = lda.N_kj[key(init_z, doc_id)];
  unint init_n_wk = lda.N_wk[key(word_id, init_z)];
  unint init_n_k = lda.N_k[init_z];
  
  lda.sampling(doc_id, word_id, 0);
  
  unint new_z = lda.get_Z(doc_id, word_id, 0);

  //これぐらいしか確認できない
  if(init_z != new_z){
    EXPECT_EQ(lda.N_kj[key(init_z, doc_id)], init_n_kj - 1);
    EXPECT_EQ(lda.N_wk[key(word_id, init_z)], init_n_wk - 1);
    EXPECT_EQ(lda.N_k[init_z], init_n_k - 1);
  }else{
    EXPECT_EQ(lda.N_kj[key(init_z, doc_id)], init_n_kj);
    EXPECT_EQ(lda.N_wk[key(word_id, init_z)], init_n_wk);
    EXPECT_EQ(lda.get_Z(doc_id, word_id, 0), init_z);
  }
}

TEST(lda, Testoutput){
  unint K = 5;
  LDA lda(K/50, 0.1, K);
  lda.read_file("../patent/test.txt");
  lda.all_sampling(10);
  lda.output("../patent/dummy.txt", 10);
  //まずはN_jの確認
  EXPECT_EQ(6, lda.N_j[1]);
  EXPECT_EQ(5, lda.N_j[2]);
}


int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

