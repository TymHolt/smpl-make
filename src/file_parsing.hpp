#ifndef FILE_PARSING_HPP
#define FILE_PARSING_HPP

#include <vector>
#include <string>
#include <util/string_parsing.hpp>

namespace smpl {

    enum LineType {
        LT_NONE,
        LT_VAR,         // var [name] = [value]
        LT_GOAL_BEGIN,  // goal [name] {
        LT_GOAL_END,    // }
        LT_SYS_COMMAND, // [value]
        LT_CD_COMMAND   // @[value]
    };

    class Line {
        private:
            size_t m_line_nr;
            LineType m_type;
            std::vector<std::string> m_values;
        public:
            Line(size_t line_nr = 0, LineType type = LT_NONE, std::vector<std::string> values = {});
            size_t GetLineNr();
            LineType GetType();
            std::vector<std::string> GetValues();
    };

    enum FileParserState {
        FPS_SEARCH_DEFINITION,
        FPS_READING_GOAL_BLOCK
    };

    class FileParser {
        private:
            size_t m_line_nr;
            util::StringParser m_string_parser;
            FileParserState m_state;
            Line ParseSourceLine(std::string line);
            Line ParseVarLine(std::string identifier);
            Line ParseGoalLine(std::string identifier);
            Line ParseGoalContentLine();
        public:
            FileParser();
            std::vector<Line> Parse(std::string source);
    };
}

#endif