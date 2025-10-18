#include <stdexcept>
#include <iostream>
#include <file_parsing.hpp>

// ---------- Line ----------

smpl::Line::Line(size_t line_nr, LineType type, std::vector<std::string> values) {
    m_line_nr = line_nr;
    m_type = type;
    m_values = values;
}

size_t smpl::Line::GetLineNr() {
    return m_line_nr;
}

smpl::LineType smpl::Line::GetType() {
    return m_type;
}

std::vector<std::string> smpl::Line::GetValues() {
    return m_values;
}

// ---------- FileParser ----------

smpl::FileParser::FileParser() {
    m_state = FPS_SEARCH_DEFINITION;
    m_line_nr = 0;
}

std::vector<smpl::Line> smpl::FileParser::Parse(std::string source) { 
    try {
        std::vector<smpl::Line> lines = {};
        std::string current_line = "";
        for (size_t index = 0; index < source.length(); index++) {
            char current_char = source.at(index);

            if (current_char == '\r')
                continue;

            if (current_char == '\n') {
                lines.push_back(ParseSourceLine(current_line));
                current_line = "";
                continue;
            }
        
            current_line += current_char;
        }

        // In case the last line does not terminate with \n
        if (current_line.length() != 0)
            lines.push_back(ParseSourceLine(current_line));
            
        if (m_state != FPS_SEARCH_DEFINITION)
            throw std::runtime_error(std::string("Missing '}'"));

        return lines;
    } catch (const std::runtime_error& exception) {
        std::cerr << "Line " << m_line_nr << ": " << exception.what() << std::endl;
        throw std::runtime_error(std::string("Parsing failed"));
    }
}

smpl::Line smpl::FileParser::ParseSourceLine(std::string line_content) {
    m_string_parser.SetContent(line_content);
    m_line_nr++;

    // Line is empty or comment
    if (!m_string_parser.HasMoreTokens()) {
        std::vector<std::string> values = {};
        Line line(m_line_nr, LT_NONE, values);
        return line;
    }

    std::string type;
    std::string identifier;
    switch (m_state) {
        case FPS_SEARCH_DEFINITION:
            type = m_string_parser.NextToken();

            if (!m_string_parser.HasMoreTokens()) 
                throw std::runtime_error(std::string("Expected identifier after " + type));

            identifier = m_string_parser.NextToken();

            if (type == "var")
                return ParseVarLine(identifier);

            if (type == "goal")
                return ParseGoalLine(identifier);

            // Unknown type
            throw std::runtime_error(std::string("Unknown type " + type));
    
        case FPS_READING_GOAL_BLOCK:
            return ParseGoalContentLine();

        default:
            throw std::runtime_error(std::string("(Internal) Illegal parsing state"));
    }
}

smpl::Line smpl::FileParser::ParseVarLine(std::string identifier) {
    if (!m_string_parser.HasMoreTokens() || m_string_parser.NextToken() != "=")
        throw std::runtime_error(std::string("Expected '=' after identifier"));

    std::string value = m_string_parser.GetRemaining();
    std::vector<std::string> values = {identifier, value};
    Line line(m_line_nr, LT_VAR, values);
    return line;
}

smpl::Line smpl::FileParser::ParseGoalLine(std::string identifier) {
    if (!m_string_parser.HasMoreTokens() || m_string_parser.NextToken() != "{")
        throw std::runtime_error(std::string("Expected '{' after " + identifier));

    if (m_string_parser.HasMoreTokens())
        throw std::runtime_error(std::string("Expected line to end after '{'"));

    m_state = FPS_READING_GOAL_BLOCK;

    std::vector<std::string> values = {identifier};
    Line line(m_line_nr, LT_GOAL_BEGIN, values);
    return line;
}

smpl::Line smpl::FileParser::ParseGoalContentLine() {
    if (m_string_parser.PeekToken() == "}") {
        m_state = FPS_SEARCH_DEFINITION;
        std::vector<std::string> values = {};
        Line line(m_line_nr, LT_GOAL_END, values);
        return line;
    }

    if (m_string_parser.PeekNonWhitespaceChar() == '@') {
        m_string_parser.SkipAfterChar('@');
        
        std::string directory_path = m_string_parser.GetRemaining();
        std::vector<std::string> values = {directory_path};
        Line line(m_line_nr, LT_CD_COMMAND, values);
        return line;
    }

    std::string command = m_string_parser.GetRemaining();
    std::vector<std::string> values = {command};
    Line line(m_line_nr, LT_SYS_COMMAND, values);
    return line;
}