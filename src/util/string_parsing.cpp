#include <string>
#include <util/string_parsing.hpp>

std::string ReadUntilAfter(std::string string, char split_char, size_t *index) {
    std::string result = "";

    for (; *index < string.length(); (*index)++) {
        char current_char = string.at(*index);

        if (current_char == split_char) {
            (*index)++;
            break;
        }

        result += current_char;
    }

    return result;
}

void util::SplitAtChar(std::string string, char split_char, std::string *part_left,
    std::string *part_right) {
    
    size_t index = 0;
    *part_left = ReadUntilAfter(string, split_char, &index);
    *part_right = ReadUntilAfter(string, split_char, &index);
}

bool util::IsWhiteSpace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool util::IsAlpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool util::IsNum(char c) {
    return c >= '0' && c <= '9';
}

bool util::IsAlphaNum(char c) {
    return IsAlpha(c) || IsNum(c);
}

bool util::IsAlphaNumExtended(char c) {
    return IsAlphaNum(c) || c == '_' || c == '-';
}