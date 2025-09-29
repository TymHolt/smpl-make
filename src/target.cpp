#include <target.hpp>
#include <string.h>
#include <fstream>
#include <iostream>
#include <parse.hpp>

SmplTarget::SmplTarget() {
    // Assign default values
    m_file_name = "Makefile.smpl";
    m_goal_name = "main";
}

SmplTarget::~SmplTarget() {

}

void SmplTarget::ParseArgument(char *argument) {
    std::string token_left = "";
    size_t index = 0;
    size_t length = strlen(argument);

    for (; index < length; index++) {
        char current_char = argument[index];
    
        if (current_char == ':') {
            index++;
            break;
        }

        token_left += current_char;
    }

   
    if (index >= length) {
        // Test if file exists, otherwise use as goal name
        std::fstream file(token_left);
        if (file.is_open()) {
            m_file_name = token_left;
            file.close();
        } else
            m_goal_name = token_left;
        
        return;
    }

    std::string token_right = "";
    for (; index < length; index++) {
        char current_char = argument[index];
        token_right += current_char;
    }

    m_file_name = token_left;
    m_goal_name = token_right;
}

bool SmplTarget::Run() {
    std::cout << "Running goal " << m_file_name << ":" << m_goal_name << std::endl;

    std::fstream file(m_file_name);
    if (!file.is_open()) {
        std::cout << "Unable to open file " << m_file_name << std::endl;
        return false;
    }

    SmplParser parser;
    size_t line_nr = 0;
    std::string line;
    bool error = false;
    while (std::getline(file, line)) {
        line_nr++;
        
        try {
            parser.ParseLine(line);
        } catch (const std::runtime_error& exception) {
            std::cout << "Line " << line_nr << ": " << exception.what() << std::endl;
            error = true;
            break;
        } catch (...) {
            std::cout << "Line " << line_nr << ": Unknown exception" << std::endl;
            error = true;
            break;
        }
    }

    file.close();

    if (error)
        return false;

    for (SmplVariable variable : parser.GetVariables()) {
        std::cout << "Variable " << variable.m_name << " -> " << variable.m_value << std::endl;
    }

    for (SmplGoal goal : parser.GetGoals()) {
        //std::cout << "Goal : " << goal.m_name << std::endl;
        
        if (goal.m_name == m_goal_name) {
            for (std::string command : goal.m_commands) {
                std::cout << "Command: " << command << std::endl;
            }

            return true;
        }
    }

    std::cout << "Goal not found" << std::endl;
    return false;    
}

std::string SmplTarget::GetFileName() {
    return m_file_name;
}

std::string SmplTarget::GetGoalName() {
    return m_goal_name;
}

std::string SmplTarget::GetFullName() {
    return GetFileName() + ':' + GetGoalName();
}