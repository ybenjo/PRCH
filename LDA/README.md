# Latent Dirichlet Allocation (Collapsed Gibbs Sampling)
## 概要
Collapsed Gibbs SamplingによるLatent Dirichlet Allocation

## 使い方
make main  
./main -[options]

### 実装
* gcc 4.2.1, googletest

### 入力ファイル
入力ファイルは以下の形式のタブ区切りテキストを用いる。  
doc_id \t word_id \t count  
ex.  
1\t1\t4  
1\t2\t2  
2\t1\t3  
  
また、辞書ファイルは以下の形式のタブ区切りテキストを用いる。  
word_id \t word  
ex.  
1\t日本語  
2\tダンコガイ  
3\tPRML  
  
出力ファイルは以下のような形式のカンマ区切りテキストである。  
\# \d's topic  
word_id_1,prob_id_1  
word_id_2,prob_id_2
word_id_3,prob_id_3

### 引数
* -i:入力ファイル
* -o:出力ファイル
* -t:サンプリング回数。省略時は100
* -l:出力時の件数。省略時は10
* -a:サンプリングパラメータ \alpha。省略時は50 / k
* -b:サンプリングパラメータ \beta。省略時は0.1
* -d:辞書ファイル。形式はword_id \t word。省略時はidがそのまま出力される。
* -x:追加した場合デバッグ用出力

## 参考文献
Griffiths TL, Steyvers M. Finding scientific topics. Proceedings of the National Academy of Sciences of the United States of America. 2004;101 Suppl 1:5228-35.  
Porteous I, Newman D, Ihler A, et al. Fast collapsed gibbs sampling for latent dirichlet allocation. New York, New York, USA: ACM Press; 2008:569.  
http://d.hatena.ne.jp/tsubosaka/20091223/1261572639  