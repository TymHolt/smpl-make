#ifndef TARGET_CPP
#define TARGET_CPP

#include <string>

class SmplTarget {
    private:
        std::string m_file_name;
        std::string m_goal_name;
    public:
        SmplTarget();
        ~SmplTarget();
        void ParseArgument(char *argument);
        bool Run();
        std::string GetFileName();
        std::string GetGoalName();
        std::string GetFullName();   
};

#endif