#ifndef STRING_PARSE_HPP
#define STRING_PARSE_HPP

#include <string>
#include <vector>

namespace util {

    void SplitAtChar(std::string string, char split_char, std::string *part_left,
        std::string *part_right);
    bool IsWhiteSpace(char c);
    bool IsAlpha(char c);
    bool IsNum(char c);
    bool IsAlphaNum(char c);
    bool IsAlphaNumExtended(char c);
}

#endif