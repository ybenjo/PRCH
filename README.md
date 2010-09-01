# PRCH (Pu'ercha, 普洱茶)
## 概要
PRCH(Patent aRChiver)は日本における特許, とりわけ製薬分野における特許マイニングを目的としたスクリプト群です。

## データ
分析には特許庁が販売しているDVD-ROM 特許公報を用いています。  
具体的には, 平成16年度から平成20年度にかけて発行された特許公報が収録されたxmlファイルを対象にしています。

## 実行環境
以下の環境で実装を行っています。  
* Ruby 1.8.7
  * RubyGems, Hpricot
* g++
  * googletest
* Tokyo Cabinet, Ruby API

## クレジット
* [ライフサイエンス統合データベースセンター](http://dbcls.rois.ac.jp/)
* [togofarm](http://g86.dbcls.jp/)
