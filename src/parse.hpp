#ifndef PARSE_HPP
#define PARSE_HPP

#include <string>

class Parser {
    private:
        std::string m_content;
        size_t m_index;
    public:
        Parser();
        ~Parser();
        void SetContent(std::string content);
        bool HasMoreTokens();
        std::string NextToken();
};

#endif