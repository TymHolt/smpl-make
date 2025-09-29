#include <parse.hpp>

SmplParser::SmplParser() {
    m_parsing_goal = false;
}

SmplParser::~SmplParser() {

}

void SmplParser::ParseLine(std::string line) {
    m_parser.SetContent(line);

    if (!m_parser.HasMoreTokens())
        return;

    if (!m_parsing_goal) {
        std::string type = m_parser.NextToken();

        if (!m_parser.HasMoreTokens()) 
            throw "Expected identifier";

        std::string identifier = m_parser.NextToken();

        if (type == "var") {
            ParseVarLine(line);
            return;
        }

        if (type == "goal") {
            ParseGoalLine(line);
            return;    
        }

        throw "Unknonw type " + type;
    } else
        ParseGoalContentLine();
}

void SmplParser::ParseVarLine(std::string identifier) {
    if (!m_parser.HasMoreTokens() || m_parser.NextToken() != "=")
        throw "Expected =";

    // Parse...
}

void SmplParser::ParseGoalLine(std::string identifier) {
    if (!m_parser.HasMoreTokens() || m_parser.NextToken() != "{")
        throw "Expected {";

    if (m_parser.HasMoreTokens())
        throw "Expected line end";

    m_parsing_goal = true;
}

void SmplParser::ParseGoalContentLine() {
   if (m_parser.NextToken() == "}") {
        m_parsing_goal = false;
        return;
   }
}

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