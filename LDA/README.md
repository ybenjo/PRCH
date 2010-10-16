# Latent Dirichlet Allocation
## 概要
二つのアルゴリズムを実装しています

* Latent Dirichlet Allocation (Collapsed Gibbs Sampling, 以下LDA)  
* Topics over Time: A Non-Markov Continuous-Time Model of Topical Trends (以下TOT)  

## 使い方
	make main  
	./main -[options]

### 引数
* -i:入力ファイル
* -o:出力ファイル
* -m:モード。0 => LDA, 1 => TOT
* -t:サンプリング回数。省略時は100
* -l:出力時の件数。省略時は10
* -a:サンプリングパラメータ \alpha。省略時は50 / k
* -b:サンプリングパラメータ \beta。省略時は0.1
* -d:辞書ファイル。形式はword_id \t word。省略時はidがそのまま出力されます
* -v:追加した場合デバッグ用出力

### 実装
* gcc 4.2.1, googletest

### 入力ファイル
#### LDA
入力ファイルは以下の形式のタブ区切りテキストを用います  
doc_id \t word_id \t count  
ex.  
	1\t1\t4  
	1\t2\t2  
	2\t1\t3  

#### TOT
TOTは時間情報を考慮するモデルであるため、LDAとは異なり以下の形式のタブ区切りテキストを用います  
doc_id \t timestamp \t word_id \t count  
ex.  
	1\t1990\t1\t4  
	1\t2005\t2\t2  
	2\t2001\t1\t3

	1\tunix_time\t1\t4  
	1\tunix_time\t2\t2  
	2\tunix_time\t1\t3
	
	1\tsec\t1\t4  
	1\tsec\t2\t2  
	2\tsec\t1\t3

timestampはドキュメントに紐付けられた時間情報です。内部で[0, 1]にするので1ファイルにおけるtimestampは統一された整数であれば何を用いても構いません。  

また、辞書ファイルは以下の形式のタブ区切りテキストを用います。これは単語idと単語文字列を紐付けるものです  
	1\t日本語  
	2\tダンコガイ  
	3\tPRML  
  
出力ファイルは以下のような形式のカンマ区切りテキストです  
	topic_id,word_id_1,prob_id_1  
	topic_id,word_id_2,prob_id_2
	topic_id,word_id_3,prob_id_3

TOTの場合、以下のような時間に対するトピックの頻度情報及びその比率も出力します
	topic_id,timestamp1,count
	topic_id,timestamp2,count
	topic_id,timestamp3,count


## 参考文献
Griffiths TL, Steyvers M. Finding scientific topics. Proceedings of the National Academy of Sciences of the United States of America. 2004;101 Suppl 1:5228-35.  
Porteous I, Newman D, Ihler A, et al. Fast collapsed gibbs sampling for latent dirichlet allocation. New York, New York, USA: ACM Press; 2008:569.  
Wang X, Mccallum A. Topics over Time : A Non-Markov Continuous-Time Model of Topical Trends. Time. 2006.  
http://d.hatena.ne.jp/tsubosaka/20091223/1261572639  