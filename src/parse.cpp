#include <parse.hpp>

Parser::Parser() {
    m_content = "";
    m_index = 0;
}

Parser::~Parser() {

}

void Parser::SetContent(std::string content) {
    m_content = content;
    m_index = 0;
}

bool IsWhiteSpace(char c) {
    return c == ' ' || c == '\t';
}

bool Parser::HasMoreTokens() {
    for (size_t index = m_index; index < m_content.length(); index++) {
        const char currentChar = m_content.at(index);
        
        if (currentChar == '#')
            return false;

        if (!IsWhiteSpace(currentChar))
            return true;
    }

    return false;
}

std::string Parser::NextToken() {
    std::string token = "";
    bool parseBegin = false;

    for (; m_index < m_content.length(); m_index++) {
        const char currentChar = m_content.at(m_index);

        if (currentChar == '#') {
            // Prevent interpreting the comment with another call
            m_index = m_content.length();
            break;
        }

        if (!parseBegin) {
            if (!IsWhiteSpace(currentChar)) {
                parseBegin = true;
                m_index--;
            }

            continue;
        }

        if (IsWhiteSpace(currentChar))
            break;

        token += currentChar;
    }

    return token;
}