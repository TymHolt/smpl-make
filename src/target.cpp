#include <target.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <util/parse.hpp>
#include <macro.hpp>
#include <command.hpp>

SmplTarget::SmplTarget(std::string file_name, std::string goal_name) {
    m_file_name = file_name;
    m_goal_name = goal_name;
}

SmplTarget::~SmplTarget() {

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
  
    smpl::ExecutionContext executionContext;
    executionContext.SetExecutionDirectory("");

    for (SmplVariable variable : parser.GetVariables()) {
        std::string value = CalculateVariable(variable.m_value);
        executionContext.AddVariable(variable.m_name, value);
    }

    for (SmplGoal goal : parser.GetGoals()) {
        if (goal.m_name == m_goal_name) {
            for (smpl::ICommand *command : goal.m_commands) {
                command->Execute(&executionContext);
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