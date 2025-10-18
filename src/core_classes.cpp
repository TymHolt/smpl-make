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

smpl::File::File(std::string file_name, std::vector<Line> lines) {
    m_name = file_name;

    // TODO Turn lines into objects
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
        
        smpl::FileParser file_parser;
        std::vector<smpl::Line> parsed_lines = file_parser.Parse(file_source);
        
        File file(file_name, parsed_lines);
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