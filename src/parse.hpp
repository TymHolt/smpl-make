#ifndef PARSE_HPP
#define PARSE_HPP

#include <string>
#include <vector>

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

typedef struct SmplGoalStruct {
    std::string m_name;
    std::vector<std::string> m_commands;
} SmplGoal;

class SmplParser {
    private:
        Parser m_parser;
        bool m_parsing_goal;
        std::vector<std::string> m_variables;
        std::vector<SmplGoal> m_goals;

        void ParseVarLine(std::string identifier);
        void ParseGoalLine(std::string identifier);
        void ParseGoalContentLine();
    public:
        SmplParser();
        ~SmplParser();
        void ParseLine(std::string line);
};

#endif