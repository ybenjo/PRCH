# Latent Dirichlet Allocation (Collapsed Gibbs Sampling)
## 概要
Collapsed Gibbs SamplingによるLatent Dirichlet Allocation

## 使い方
make main 
./main hogehoge

### 実装
* gcc 4.2.1, googletest

### 入力ファイル
以下の形式のタブ区切りファイルを用いる。 
doc_id \t word_id \t count 
ex. 
1\t1\t4 
1\t2\t2 
2\t1\t3 

### 引数
* -i:入力ファイル
* -o:出力ファイル
* -t:サンプリング回数
* -l:出力時の件数
* -a:サンプリングパラメータ \alpha
* -b:サンプリングパラメータ \beta

## おまけ
idだけじゃ辛いのでidと文字列の変換スクリプトがconv_id_to_word.rb 
ruby conv_id_to_word.rb [出力ファイル] [辞書ファイル] 
で変換したファイル（出力ファイル.conv）を生成。 
辞書ファイルはタブ区切りでid \t wordとする。 

## 参考文献
Griffiths TL, Steyvers M. Finding scientific topics. Proceedings of the National Academy of Sciences of the United States of America. 2004;101 Suppl 1:5228-35. 
Porteous I, Newman D, Ihler A, et al. Fast collapsed gibbs sampling for latent dirichlet allocation. New York, New York, USA: ACM Press; 2008:569. 
http://d.hatena.ne.jp/tsubosaka/20091223/1261572639 