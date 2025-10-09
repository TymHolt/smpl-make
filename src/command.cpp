#include "command.hpp"
#include <stdexcept>
#include <iostream>

smpl::ExecutionContext::ExecutionContext() {
    m_execution_directory = "";
}

void smpl::ExecutionContext::SetExecutionDirectory(std::string execution_directory) {
    m_execution_directory = execution_directory;
}

void smpl::ExecutionContext::AddVariable(std::string name, std::string value) {
    if (ContainsVariable(name))
        throw std::runtime_error(std::string("Variable " + name + " already defined"));

    Variable variable;
    variable.m_name = name;
    variable.m_value = value;
    m_variables.push_back(variable);
}

bool smpl::ExecutionContext::ContainsVariable(std::string name) {
    for (Variable variable : m_variables) {
        if (variable.m_name == name)
            return true;
    }

    return false;
}

enum ApplyVarState {
    AVS_READ_FREE,
    AVS_VAR_SYMBOL_READ,
    AVS_READ_VAR
};

std::string smpl::ExecutionContext::ApplyVariables(std::string string) {
    std::string result = "";
    ApplyVarState state = AVS_READ_FREE;
    std::string var_name = "";
    int para_count = 0;

    for (size_t index = 0; index < string.length(); index++) {
        char current_char = string.at(index);

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

                        for (Variable variable : m_variables) {
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

std::string smpl::ExecutionContext::GetExecutionDirectory() {
    return m_execution_directory;
}

smpl::ICommand::ICommand() {

}

int smpl::ICommand::Execute(ExecutionContext* context) {
    return 0;
}

smpl::SystemCommand::SystemCommand(std::string raw_command) {
    m_raw_command = raw_command;
}

int smpl::SystemCommand::Execute(ExecutionContext* context) {
    std::string command = m_raw_command;
    command = context->ApplyVariables(command);
    std::string execution_directory = context->GetExecutionDirectory();

    // Call to cd in same context if execution directory is defined
    if (execution_directory != "")
        command = "cd " + execution_directory + " && " + command;

    std::cout << command << std::endl;
    return system(command.c_str());
}