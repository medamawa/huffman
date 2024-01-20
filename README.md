# huffman

## 実行

```shell
make && bin/huffman news.txt
```

## 実装したこと

.txt ファイルをハフマン符号化で .zip ファイルに圧縮するコードと、.zip ファイルを解凍して .txt ファイルに戻すプログラムを実装した。

２つの処理は、main.c の中の main 関数で実行されているが、互いに完全に独立している。（関数や構造体も新しく定義している。）

> ただ単に、ファイルを分けるのが面倒だったからです。

## 圧縮効率

元の news.txt と news.zip を比較した。

```
news.txt    139,909 bytes

news.zip     74,167 bytes
```

約半分に圧縮することができた。

もちろん解凍の処理を挟めば、完全に元のテキストと同じファイルになります。

> すいません、少し嘘です。圧縮の末端処理は心が折れたので出来ていません。
> 
> なので、最後の文字は上手く復号できません。😢

## .zipのファイル構成

```c
unsigned char char_num
  出てきた文字が何種類か

/* このセットが char_num 回繰り返される */
char symbol, char code_len, char[256] code
  symbol: 文字
  code_len: 文字に対応するコードが何bitか
  code: 符号

/* 本文 */
bit
  エンコードした文章
```
