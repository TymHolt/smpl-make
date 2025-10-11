#ifndef CORE_CLASSES_HPP
#define CORE_CLASSES_HPP

#include <string>

namespace smpl {

    class Target {
        private:
            std::string m_file_name;
            std::string m_goal_name;
        public:
            Target(std::string file_name = "", std::string goal_name = "");
            std::string GetFileName();
            std::string GetGoalName();
    };
}

#endif