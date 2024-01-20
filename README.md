# huffman

## .zipのファイル構成

unsigned char char_num
    出てきた文字が n種類
(char symbol, char code_len, char* code) * n
    symbol: 文字
    code_len: 文字に対応するコードが何bitか
    code: コード
bit
    エンコードした文章