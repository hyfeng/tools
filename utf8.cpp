/*================================================================
*   
*   description: utf8字符串的常用函数
*
================================================================*/
#include "utf8.h"
#include <stdlib.h>
#include <cstring>
#include <cctype>

static const unsigned char utf8_0 = 0x00;
static const unsigned char utf8_1 = 0x80;
static const unsigned char utf8_2 = 0xc0;
static const unsigned char utf8_3 = 0xe0;
static const unsigned char utf8_4 = 0xf0;
static const unsigned char utf8_5 = 0xf8;

bool is_utf8_punct_int(unsigned int utf8_c) {
    if (utf8_c == 0xe28093 || utf8_c == 0xe28094 ||
            (utf8_c >= 0xe28098 && utf8_c <= 0xe2809d) ||
            (utf8_c >= 0xe29880 && utf8_c <= 0xe29bbf) ||
            (utf8_c >= 0xe29c80 && utf8_c <= 0xe29ebf) ||
            (utf8_c >= 0xe38080 && utf8_c <= 0xe380bf) ||
            (utf8_c >= 0xefb890 && utf8_c <= 0xefb89f) ||
            (utf8_c >= 0xefb8b0 && utf8_c <= 0xefb98f) ||
            (utf8_c >= 0xefbc80 && utf8_c <= 0xefbc8f) ||
            (utf8_c >= 0xefbc9a && utf8_c <= 0xefbc9f)) {
        return true;
    }
    return false;
}

/*
 * 判断一个term是否是utf8编码的标点符号
 * 输入是char编码的字节term, 长度
 * return true for a punct, false not
 * */
bool is_utf8_punct(const char* term, int len) {
    if (len == 0 || len > 4) { //大于4个字节，肯定不是一个字符
        return false;
    }
    const unsigned char* p = (const unsigned char*)term;
    unsigned int ct = 0;
    if (len == 1) {
        if (ispunct(term[0])) {
            return true;
        } else {
            return false;
        }
    }
    int cc = 0;
    if ((term[0] & utf8_1) == utf8_0) {
        cc = 1;
    } else if ((term[0] & utf8_3) == utf8_2) {
        cc = 2;
    } else if ((term[0] & utf8_4) == utf8_3) {
        cc = 3;
    } else if ((term[0] & utf8_5) == utf8_4) {
        cc = 4;
    } else {
        return false;
    }
    if (cc != len) {
        return false;
    }
    ct = p[0];
    for (int j = 1; j < cc; ++j) {
        if (j < len && (p[j] & utf8_2) == utf8_1) {
            ct = ((ct << 8) | p[j]);
        } else {
            return false;
        }
    }
    return is_utf8_punct_int(ct);
}

//返回一个utf8字符的byte数量，返回0，表示不是一个正常的utf8字
int get_utf8_char_len(const char* term, int len) {
    int cc = 0;
    if ((term[0] & utf8_1) == utf8_0) {
        cc = 1;
    } else if ((term[0] & utf8_3) == utf8_2) {
        cc = 2;
    } else if ((term[0] & utf8_4) == utf8_3) {
        cc = 3;
    } else if ((term[0] & utf8_5) == utf8_4) {
        cc = 4;
    } else {
        return 0;
    }
    if (cc > len) {
        return 0;
    }
    for (int j = 1; j < cc; ++j) {
        if ((term[j] & utf8_2) == utf8_1) {
            continue;
        } else {
            return 0;
        }
    }
    return cc;
}

/*
 * 判读一个term是否是【一个】utf8编码的字符
 * 输入是char编码的字节term，长度
 * return: true for a word; false not
 * */
bool is_utf8_char(const char* term, int len) {
    if (len == 0 || len > 4) {
        return false;
    }
    int count = 0;
    int i = 0;
    while (i < len) {
        int cc = 1;
        if ((term[i] & utf8_1) == utf8_0) {
            cc = 1;
        } else if ((term[i] & utf8_3) == utf8_2) {
            cc = 2;
        } else if ((term[i] & utf8_4) == utf8_3) {
            cc = 3;
        } else if ((term[i] & utf8_5) == utf8_4) {
            cc = 4;
        } else {
            return false;
        }
        for (int j = 1; j < cc; ++j) {
            if (j + i < len && (term[j + i] & utf8_2) == utf8_1) {
                continue;
            } else {
                return false;
            }
        }
        i += cc;
        ++count;
        if (count > 1) {
            return false;
        }
    }
    return true;
}
