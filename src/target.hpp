#ifndef TARGET_CPP
#define TARGET_CPP

#include <string>

class SmplTarget {
    private:
        std::string m_file_name;
        std::string m_goal_name;
    public:
        SmplTarget(std::string file_name = "", std::string goal_name = "");
        ~SmplTarget();
        bool Run();
        std::string GetFileName();
        std::string GetGoalName();
        std::string GetFullName();   
};

#endif