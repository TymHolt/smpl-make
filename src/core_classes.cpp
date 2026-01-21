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

smpl::Variable::Variable(std::string name) {
    m_name = name;
    m_value = "";
}

std::string smpl::Variable::GetName() {
    return m_name;
}

void smpl::Variable::SetValue(std::string value) {
    m_value = value;
}

std::string smpl::Variable::GetValue() {
    return m_value;
}

// ---------- Goal ----------

smpl::Goal::Goal(std::string name) {
    m_name = name;
}

smpl::Goal::~Goal() {

}

std::string smpl::Goal::GetName() {
    return m_name;
}

void smpl::Goal::AddSystemCommand(std::string system_command) {
    // TODO Implement
}

void smpl::Goal::AddUtilityCommand(std::string utlity_command) {
    // TODO Implement
}

// ---------- File ----------

smpl::File::File(std::string name) {
    m_name = name;
}

smpl::File::~File() {
    for (Variable *variable : m_variables)
        delete variable;
    
    for (Goal *goal : m_goals)
        delete goal;
}

std::string smpl::File::GetName() {
    return m_name;
}

smpl::Variable *smpl::File::AddVariable(std::string name) {
    if (GetVariableByName(name) != NULL)
        throw std::runtime_error(std::string("Variable already exists: " + name));
    
    smpl::Variable *variable = new Variable(name);
    m_variables.push_back(variable);
    return variable;
}

smpl::Goal *smpl::File::AddGoal(std::string name) {
    if (GetGoalByName(name) != NULL)
        throw std::runtime_error(std::string("Goal already exists: " + name));
    
    smpl::Goal *goal = new Goal(name);
    m_goals.push_back(goal);
    return goal;
}

smpl::Variable *smpl::File::GetVariableByName(std::string name) {
    for (Variable *variable : m_variables)
        if (variable->GetName() == name)
            return variable;
    
    return NULL;
}

smpl::Goal *smpl::File::GetGoalByName(std::string name) {
    for (Goal *goal : m_goals)
        if (goal->GetName() == name)
            return goal;
    
    return NULL;
}

// ---------- Context ----------

smpl::Context::~Context() {
    for (File *file : m_files)
        delete file;
}

smpl::File *smpl::Context::AddFile(std::string name) {
    if (GetFileByName(name) != NULL)
        throw std::runtime_error(std::string("File already exists: " + name));
    
    smpl::File *file = new File(name);
    m_files.push_back(file);
    return file;
}

smpl::File *smpl::Context::GetFileByName(std::string name) {
    for (File *file : m_files)
        if (file->GetName() == name)
            return file;
    
    return NULL;
}

smpl::Goal *smpl::Context::GetGoalByTarget(Target target) {
    File *file = GetFileByName(target.GetFileName());
    if (file == NULL)
        return NULL;

    Goal *goal = file->GetGoalByName(target.GetGoalName());
    if (goal == NULL)
        return NULL;

    return goal;
}