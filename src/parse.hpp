#ifndef PARSE_HPP
#define PARSE_HPP

#include <string>
#include <vector>
#include "command.hpp"

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
        std::string PeekToken();
        char PeekNonWhitespaceChar();
        void SkipAfterChar(char c);
        std::string GetRemaining();
};

typedef struct SmplVariableStruct {
    std::string m_name;
    std::string m_value;
} SmplVariable;

typedef struct SmplGoalStruct {
    std::string m_name;
    std::vector<smpl::ICommand *> m_commands;
} SmplGoal;

class SmplParser {
    private:
        Parser m_parser;
        bool m_parsing_goal;
        SmplGoal m_current_goal;
        std::vector<SmplVariable> m_variables;
        std::vector<SmplGoal> m_goals;

        void ParseVarLine(std::string identifier);
        void ParseGoalLine(std::string identifier);
        void ParseGoalContentLine();
    public:
        SmplParser();
        ~SmplParser();
        void ParseLine(std::string line);
        std::vector<SmplVariable> GetVariables();
        std::vector<SmplGoal> GetGoals();
};

#endif