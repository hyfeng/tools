/*================================================================
*   description: utf8字符串的常用函数
*
================================================================*/
#ifndef OPPO_UTF8_OPERATOE_H_
#define OPPO_UTF8_OPERATOE_H_

/*
 * 判断一个term是否是utf8编码的标点符号
 * 输入是char编码的字节term, 长度
 * return true for a punct, false not
 * */
bool is_utf8_punct(const char* term, int len);

/*
 * 判读一个term是否是【一个】utf8编码的字符
 * 输入是char编码的字节term，长度
 * return: true for a word; false not
 * */
bool is_utf8_char(const char* term, int len);

//返回一个utf8字符的byte数量，返回0，表示不是一个正常的utf8字
int get_utf8_char_len(const char* term, int len);

#endif

