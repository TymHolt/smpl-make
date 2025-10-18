#include <stdexcept>
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

util::StringParser::StringParser() {
    m_content = "";
    m_index = 0;
}

void util::StringParser::SetContent(std::string content) {
    m_content = content;
    m_index = 0;
}

bool IsWhiteSpace(char c) {
    return c == ' ' || c == '\t';
}

bool util::StringParser::HasMoreTokens() {
    for (size_t index = m_index; index < m_content.length(); index++) {
        const char current_char = m_content.at(index);
        
        if (current_char == '#')
            return false;

        if (!IsWhiteSpace(current_char))
            return true;
    }

    return false;
}

std::string util::StringParser::NextToken() {
    std::string token = "";
    bool parse_begin = false;

    for (; m_index < m_content.length(); m_index++) {
        const char current_char = m_content.at(m_index);

        if (current_char == '#') {
            // Prevent interpreting the comment with another call
            m_index = m_content.length();
            break;
        }

        if (!parse_begin) {
            if (!IsWhiteSpace(current_char)) {
                parse_begin = true;
                m_index--;
            }

            continue;
        }

        if (IsWhiteSpace(current_char))
            break;

        token += current_char;
    }

    return token;
}

std::string util::StringParser::PeekToken() {
    std::string token = "";
    bool parse_begin = false;

    for (size_t index = m_index; index < m_content.length(); index++) {
        const char current_char = m_content.at(index);

        if (current_char == '#')
            break;

        if (!parse_begin) {
            if (!IsWhiteSpace(current_char)) {
                parse_begin = true;
                index--;
            }

            continue;
        }

        if (IsWhiteSpace(current_char))
            break;

        token += current_char;
    }

    return token;
}

char util::StringParser::PeekNonWhitespaceChar() {
     for (size_t index = m_index; index < m_content.length(); index++) {
        const char current_char = m_content.at(index);
        
        if (!IsWhiteSpace(current_char))
            return current_char;
    }

    return 0;
}

void util::StringParser::SkipAfterChar(char c) {
    for (; m_index < m_content.length(); m_index++) {
        const char current_char = m_content.at(m_index);
        
        if (current_char == c) {
            m_index++; // Skip this char also
            return;
        }
    }    
}

std::string util::StringParser::GetRemaining() {
    std::string result = "";
    bool whitespace_skippped = false;

    for (; m_index < m_content.length(); m_index++) {
        const char current_char = m_content.at(m_index);

        if (current_char == '#') {
            // Prevent interpreting the comment with another call
            m_index = m_content.length();
            break;
        }

        if (!whitespace_skippped) {
            if (!IsWhiteSpace(current_char)) {
                whitespace_skippped = true;
                m_index--;
            }

            continue;
        }

        result += current_char;
    }

    return result;
}