#include <stdexcept>
#include <util/parse.hpp>
#include <util/file.hpp>
#include <core_classes.hpp>

// ---------- Target ----------

smpl::Target::Target(std::string argument) {
    // Default values
    m_file_name = "Makefile.smpl";
    m_goal_name = "main";

    if (argument == "")
        return;

    std::string part_left;
    std::string part_right;
    util::SplitAtChar(std::string(argument), ':', &part_left, &part_right);

    // Only one name given, test if file or goal name is meant
    if (part_right == "") {
        if (util::FileExists(part_left))
            m_file_name = part_left;
        else
            m_goal_name = part_left;
    } else {
        m_file_name = part_left;
        m_goal_name = part_right;
    }
}

std::string smpl::Target::GetFileName() {
    return m_file_name;
}

std::string smpl::Target::GetGoalName() {
    return m_goal_name;
}

// ---------- Variable ----------

smpl::Variable::Variable(std::string name, std::string value) {
    m_name = name;
    m_value = value;
}

std::string smpl::Variable::GetName() {
    return m_name;
}

std::string smpl::Variable::GetValue() {
    return m_value;
}

// ---------- Goal ----------

smpl::Goal::Goal(std::string name, std::vector<smpl::ICommand *> commands) {
    m_name = name;
    m_commands = commands;
}

std::string smpl::Goal::GetName() {
    return m_name;
}

std::vector<smpl::ICommand *> smpl::Goal::GetCommands() {
    return m_commands;
}

// ---------- File ----------

smpl::File::File(std::string file_name) {
    m_name = file_name;
}

std::string smpl::File::GetName() {
    return m_name;
}

std::vector<smpl::Variable> smpl::File::Variables() {
    return m_variables;    
}           

std::vector<smpl::Goal> smpl::File::GetGoals() {
    return m_goals;
}

// ---------- ExecutionContext ----------

smpl::ExecutionContext::ExecutionContext() {
    m_execution_directory = "";
}

void smpl::ExecutionContext::SetExecutionDirectory(std::string execution_directory) {
    m_execution_directory = execution_directory;
}

void smpl::ExecutionContext::AddVariable(std::string name, std::string value) {
    if (ContainsVariable(name))
        throw std::runtime_error(std::string("Variable " + name + " already defined"));

    Variable variable(name, value);
    m_variables.push_back(variable);
}

bool smpl::ExecutionContext::ContainsVariable(std::string name) {
    for (Variable variable : m_variables) {
        if (variable.GetName() == name)
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
                            if (variable.GetName() == var_name) {
                                result += variable.GetValue();
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

// ---------- ICommand ----------

smpl::ICommand::ICommand() {

}

int smpl::ICommand::Execute(ExecutionContext *context) {
    return 0;
}

// ---------- Context ----------

smpl::Context::Context(std::vector<Target> targets) {
    
}