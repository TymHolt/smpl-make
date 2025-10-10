#include <parse.hpp>
#include <stdexcept>

SmplParser::SmplParser() {
    m_parsing_goal = false;
}

SmplParser::~SmplParser() {
    for (SmplGoal goal : m_goals) {
        for (smpl::ICommand *command : goal.m_commands)
            delete command;
    }
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

    if (m_parser.PeekNonWhitespaceChar() == '@') {
        m_parser.SkipAfterChar('@');
        
        std::string directory_path = m_parser.GetRemaining();
        smpl::ChangeDirectoryCommand *command = new smpl::ChangeDirectoryCommand(directory_path);
        m_current_goal.m_commands.push_back(command);
        return;
    }

    std::string raw_command = m_parser.GetRemaining();
    smpl::SystemCommand *system_command = new smpl::SystemCommand(raw_command);
    m_current_goal.m_commands.push_back(system_command);
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
        const char current_char = m_content.at(index);
        
        if (current_char == '#')
            return false;

        if (!IsWhiteSpace(current_char))
            return true;
    }

    return false;
}

std::string Parser::NextToken() {
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

std::string Parser::PeekToken() {
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

char Parser::PeekNonWhitespaceChar() {
     for (size_t index = m_index; index < m_content.length(); index++) {
        const char current_char = m_content.at(index);
        
        if (!IsWhiteSpace(current_char))
            return current_char;
    }

    return 0;
}

void Parser::SkipAfterChar(char c) {
    for (; m_index < m_content.length(); m_index++) {
        const char current_char = m_content.at(m_index);
        
        if (current_char == c) {
            m_index++; // Skip this char also
            return;
        }
    }    
}

std::string Parser::GetRemaining() {
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