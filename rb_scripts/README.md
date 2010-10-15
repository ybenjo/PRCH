## 自分用スクリプト説明
* parser  
** xmlをparseするためのスクリプト  
* combine_all_lda.cc  
** 重複bag of wordsから重複を除去し出力。同時に時間情報も付与  
* db_util.rb  
** TCを開くためのアレ。もう使ってない  
* make_lda_files.rb  
** LDA用にTCを叩いて単語辞書、ドキュメント辞書、セクション別bag of words、重複bag of wordsを生成  
* combine_tc.rb  
** 散らばっていたTCを結合しcombine_year.hdbとして生成  
* filter_class.rb  
** 単語フィルター定義  
* word_filter.rb  
** 単語フィルター実行ファイル  
