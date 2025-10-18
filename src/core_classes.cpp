#include <stdexcept>
#include <util/string_parsing.hpp>
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

// ---------- Command ----------

smpl::Command::Command(CommandType type, std::string value) {
    m_type = type;
    m_value = value;
}

smpl::CommandType smpl::Command::GetType() {
    return m_type;
}

std::string smpl::Command::GetValue() {
    return m_value;
}

// ---------- Goal ----------

smpl::Goal::Goal(std::string name, std::vector<Command> commands) {
    m_name = name;
    m_commands = commands;
}

std::string smpl::Goal::GetName() {
    return m_name;
}

std::vector<smpl::Command> smpl::Goal::GetCommands() {
    return m_commands;
}

// ---------- File ----------

smpl::File::File(std::string file_name,  std::vector<Variable> variables,
        std::vector<Goal> goals) {
    
    m_name = file_name;
    m_variables = variables;
    m_goals = goals;
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

// ---------- FileConverter ----------

smpl::FileConverter::FileConverter() {

}

smpl::File smpl::FileConverter::Convert(std::string file_name, std::vector<Line> lines) {
    for (Line line : lines) {
        switch (line.GetType()) {
            case LT_NONE:
                break;
            
            case LT_VAR:
                HandleVarLine(line);
                break;

            case LT_GOAL_BEGIN:
                HandleGoalBeginLine(line);
                break;

            case LT_GOAL_END:
                HandleGoalEndLine(line);
                break;

            case LT_SYS_COMMAND:
                HandleSysCommandLine(line);
                break;

            case LT_CD_COMMAND:
                HandleCdCommandLine(line);
                break;

            default:
                throw std::runtime_error(std::string("Unknown line type"));
        }
    }

    if (m_in_goal_block)
        throw std::runtime_error(std::string("Goal block without end"));

    File file(file_name, m_variables, m_goals);
    return file;
}

void smpl::FileConverter::HandleVarLine(Line line) {
    if (m_in_goal_block)
        throw std::runtime_error(std::string("Variable can not be defined inside of a goal"));

    std::string name = line.GetValues()[0];
    std::string value = line.GetValues()[1];

    if (ContainsVariable(name))
        throw std::runtime_error(std::string("Variable " + name + " defined again"));

    AddVariable(name, value);
}

void smpl::FileConverter::HandleGoalBeginLine(Line line) {
    if (m_in_goal_block)
        throw std::runtime_error(std::string("Goal can not be defined inside of a goal"));

    m_in_goal_block = true;
    m_goal_name = line.GetValues()[0];
    m_goal_commands = {};

    if (ContainsGoal(m_goal_name))
        throw std::runtime_error(std::string("Goal " + m_goal_name + " defined again"));
}

void smpl::FileConverter::HandleGoalEndLine(Line line) {
    if (!m_in_goal_block)
        throw std::runtime_error(std::string("Unexpected block end"));

    m_in_goal_block = false;
    AddGoal(m_goal_name, m_goal_commands);
}

void smpl::FileConverter::HandleSysCommandLine(Line line) {
    if (!m_in_goal_block)
        throw std::runtime_error(std::string("Unexpected command"));

    Command command(CT_SYSTEM, line.GetValues()[0]);
    m_goal_commands.push_back(command);
}

void smpl::FileConverter::HandleCdCommandLine(Line line) {
    if (!m_in_goal_block)
        throw std::runtime_error(std::string("Unexpected command"));

    Command command(CT_CHANGE_DIRECTORY, line.GetValues()[0]);
    m_goal_commands.push_back(command);
}

bool smpl::FileConverter::ContainsVariable(std::string name) {
    for (Variable variable : m_variables) {
        if (variable.GetName() == name)
            return true;
    }

    return false;
}

bool smpl::FileConverter::ContainsGoal(std::string name) {
    for (Goal goal : m_goals) {
        if (goal.GetName() == name)
            return true;
    }

    return false;
}

void smpl::FileConverter::AddVariable(std::string name, std::string value) {
    Variable variable(name, value);
    m_variables.push_back(variable);
}

void smpl::FileConverter::AddGoal(std::string name, std::vector<Command> commands) {
    Goal goal(name, commands);
    m_goals.push_back(goal);
}

// ---------- Context ----------

smpl::Context::Context(std::vector<Target> targets) {
    m_targets = targets;
    m_files = {};

    for (Target target : m_targets) {
        std::string file_name = target.GetFileName();

        if (HasFileLoaded(file_name))
            continue;

        if (!util::FileExists(file_name))
            throw std::runtime_error(std::string("File " + file_name + " does not exist"));

        std::string file_source = util::LoadTextFile(file_name);
        
        FileParser file_parser;
        std::vector<Line> parsed_lines = file_parser.Parse(file_source);
        
        FileConverter file_converter;
        File file = file_converter.Convert(file_name, parsed_lines);
        m_files.push_back(file);
    }
}

bool smpl::Context::HasFileLoaded(std::string name) {
    for (File file : m_files) {
        if (file.GetName() == name)
            return true;
    }

    return false;
}

bool smpl::Context::RunAll() {
    for (Target target : m_targets) {
        // TODO Execution
    }

    return true;
}
