#ifndef STRING_PARSE_HPP
#define STRING_PARSE_HPP

#include <string>
#include <vector>

namespace util {

    class StringParser {
        private:
            std::string m_content;
            size_t m_index;
        public:
            StringParser();
            void SetContent(std::string content);
            bool HasMoreTokens();
            std::string NextToken();
            std::string PeekToken();
            char PeekNonWhitespaceChar();
            void SkipAfterChar(char c);
            std::string GetRemaining();
    };

    void SplitAtChar(std::string string, char split_char, std::string *part_left,
        std::string *part_right);
}

#endif