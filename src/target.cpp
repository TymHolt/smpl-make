#include <target.hpp>
#include <string.h>
#include <fstream>
#include <iostream>
#include <parse.hpp>
#include <stdexcept>
#include <macro.hpp>

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

enum VarCalculationState {
    VCS_READ_FREE,
    VCS_MACRO_SYMBOL_READ,
    VCS_READ_MACRO
};

std::string CalculateVariable(std::string value) {
    std::string result = "";
    VarCalculationState state = VCS_READ_FREE;
    std::string macro = "";
    int para_count = 0;

    for (size_t index = 0; index < value.length(); index++) {
        char current_char = value.at(index);

        switch (state) {
            case VCS_READ_FREE:
                if (current_char == '$') {
                    state = VCS_MACRO_SYMBOL_READ;
                    break;
                }

                result += current_char;
                break;
            
            case VCS_MACRO_SYMBOL_READ:
                if (current_char == '(') {
                    state = VCS_READ_MACRO;
                    macro = "";
                    para_count = 1;
                    break;
                }

                // $ was not for a macro -> needs to be added into the result now
                result += "$" + current_char;
                state = VCS_READ_FREE;
                break;

            case VCS_READ_MACRO:
                if (current_char == '(')
                    para_count++;
                else if(current_char == ')') {
                    para_count--;
                    
                    if (para_count == 0) {
                        result += Macro::RunMacro(macro);
                        state = VCS_READ_FREE;
                    }
                } else
                    macro += current_char;
                break;

            default:
                throw std::runtime_error(std::string("(Internal) Illegal state"));
        }
    }

    // End of content
    switch(state) {
        case VCS_MACRO_SYMBOL_READ:
            result += "$";
            break;

        case VCS_READ_MACRO:
            throw std::runtime_error(std::string("Unfinished macro"));
    }

    return result;
}

enum ApplyVarState {
    AVS_READ_FREE,
    AVS_VAR_SYMBOL_READ,
    AVS_READ_VAR
};

std::string ApplyVariables(std::string command, std::vector<SmplVariable> variables) {
    std::string result = "";
    ApplyVarState state = AVS_READ_FREE;
    std::string var_name = "";
    int para_count = 0;

    for (size_t index = 0; index < command.length(); index++) {
        char current_char = command.at(index);

        switch (state) {
            case AVS_READ_FREE:
                if (current_char == '%') {
                    state = AVS_VAR_SYMBOL_READ;
                    break;
                }

                result += current_char;
                break;
            
            case AVS_VAR_SYMBOL_READ:
                if (current_char == '(') {
                    state = AVS_READ_VAR;
                    var_name = "";
                    para_count = 1;
                    break;
                }

                // % was not for a var -> needs to be added into the result now
                result += "%" + current_char;
                state = AVS_READ_FREE;
                break;

            case AVS_READ_VAR:
                if (current_char == '(')
                    para_count++;
                else if(current_char == ')') {
                    para_count--;
                    
                    if (para_count == 0) {
                        state = AVS_READ_FREE;
                        bool found = false;

                        for (SmplVariable variable : variables) {
                            if (variable.m_name == var_name) {
                                result += variable.m_value;
                                found = true;
                            }
                        }

                        if (!found)
                            throw std::runtime_error(std::string("Unknown variable") + var_name);
                    }
                } else
                    var_name += current_char;
                break;

            default:
                throw std::runtime_error(std::string("(Internal) Illegal state"));
        }
    }

    // End of content
    switch(state) {
        case AVS_VAR_SYMBOL_READ:
            result += "$";
            break;

        case AVS_READ_VAR:
            throw std::runtime_error(std::string("Unfinished var name"));
    }

    return result;
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
  
    std::vector<SmplVariable> variables = parser.GetVariables();
    for (SmplVariable& variable : variables) {
        variable.m_value = CalculateVariable(variable.m_value);
        std::cout << "Variable " << variable.m_name << " -> " << variable.m_value << std::endl;
    }

    for (SmplGoal goal : parser.GetGoals()) {
        if (goal.m_name == m_goal_name) {
            for (std::string command : goal.m_commands) {
                std::string applied_command = ApplyVariables(command, variables);
                std::cout << "Command: " << applied_command << std::endl;
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