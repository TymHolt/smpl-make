#include <parse.hpp>
#include <stdexcept>

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
            throw std::runtime_error(std::string("Expected identifier"));

        std::string identifier = m_parser.NextToken();

        if (type == "var") {
            ParseVarLine(identifier);
            return;
        }

        if (type == "goal") {
            ParseGoalLine(identifier);
            return;    
        }

        throw std::runtime_error(std::string("Expected identifier") + type);
    } else
        ParseGoalContentLine();
}

void SmplParser::ParseVarLine(std::string identifier) {
    if (!m_parser.HasMoreTokens() || m_parser.NextToken() != "=")
        throw std::runtime_error(std::string("Expected ="));

    for (SmplVariable variable : m_variables) {
        if (variable.m_name == identifier)
           throw std::runtime_error(std::string("Duplicate variable identifier ") + identifier);
    }

    SmplVariable variable;
    variable.m_name = identifier;
    variable.m_value = m_parser.GetRemaining();
    
    m_variables.push_back(variable);
}

void SmplParser::ParseGoalLine(std::string identifier) {
    if (!m_parser.HasMoreTokens() || m_parser.NextToken() != "{")
        throw std::runtime_error(std::string("Expected {}"));

    if (m_parser.HasMoreTokens())
        throw std::runtime_error(std::string("Expected line end"));

    for (SmplGoal goal : m_goals) {
        if (goal.m_name == identifier)
           throw std::runtime_error(std::string("Duplicate goal identifier ") + identifier);
    }

    m_parsing_goal = true;
    m_current_goal.m_name = identifier;
    m_current_goal.m_commands = {};
}

void SmplParser::ParseGoalContentLine() {
   if (m_parser.PeekToken() == "}") {
        m_parsing_goal = false;
        m_goals.push_back(m_current_goal);
        return;
   }

   m_current_goal.m_commands.push_back(m_parser.GetRemaining());
}

std::vector<SmplVariable> SmplParser::GetVariables() {
    return m_variables;
}

std::vector<SmplGoal> SmplParser::GetGoals() {
    return m_goals;
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

std::string Parser::PeekToken() {
    std::string token = "";
    bool parseBegin = false;

    for (size_t index = m_index; index < m_content.length(); index++) {
        const char currentChar = m_content.at(index);

        if (currentChar == '#')
            break;

        if (!parseBegin) {
            if (!IsWhiteSpace(currentChar)) {
                parseBegin = true;
                index--;
            }

            continue;
        }

        if (IsWhiteSpace(currentChar))
            break;

        token += currentChar;
    }

    return token;
}

std::string Parser::GetRemaining() {
    std::string result = "";
    bool whitespaceSkippped = false;

    for (; m_index < m_content.length(); m_index++) {
        const char currentChar = m_content.at(m_index);

        if (currentChar == '#') {
            // Prevent interpreting the comment with another call
            m_index = m_content.length();
            break;
        }

        if (!whitespaceSkippped) {
            if (!IsWhiteSpace(currentChar)) {
                whitespaceSkippped = true;
                m_index--;
            }

            continue;
        }

        result += currentChar;
    }

    return result;
}