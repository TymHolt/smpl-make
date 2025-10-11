#include <core_classes.hpp>

smpl::Target::Target(std::string file_name, std::string goal_name) {
    m_file_name = file_name;
    m_goal_name = goal_name;
}

std::string smpl::Target::GetFileName() {
    return m_file_name;
}

std::string smpl::Target::GetGoalName() {
    return m_goal_name;
}