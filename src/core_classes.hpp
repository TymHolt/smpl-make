#ifndef CORE_CLASSES_HPP
#define CORE_CLASSES_HPP

#include <string>
#include <vector>

namespace smpl {

    class Target {
        private:
            std::string m_file_name;
            std::string m_goal_name;
        public:
            Target(std::string argument = "");
            std::string GetFileName();
            std::string GetGoalName();
    };

    class Variable {
        private:
            std::string m_name;
            std::string m_value;
        public:
            Variable(std::string name = "");
            std::string GetName();
            // TODO Variable must also contain macro / variale fetches
            // So GetValue() might also need context access
            void SetValue(std::string value);
            std::string GetValue();
    };

    // TODO This is a dummy
    class Command {
        private:
            std::string m_raw_command;
        public:
            Command(std::string raw_command);
            void Execute();
    };

    class Goal {
        private:
            std::string m_name;
            // TODO
            // Commands will contain variable / macro fetches
            std::vector<Command *> m_commands;
        public:
            Goal(std::string name = "");
            ~Goal();
            std::string GetName();
            void AddSystemCommand(std::string system_command);
            void AddUtilityCommand(std::string utility_command);
            void Execute();
    };

    class File {
        private:
            std::string m_name;
            std::vector<Variable *> m_variables;
            std::vector<Goal *> m_goals;
        public:
            File(std::string name = "");
            ~File();
            std::string GetName();
            Variable *AddVariable(std::string name);
            Goal *AddGoal(std::string name);
            Variable *GetVariableByName(std::string name);
            Goal *GetGoalByName(std::string name);
    };

    class Context {
        private:
            std::vector<File *> m_files;
        public:
            ~Context();
            File *AddFile(std::string name);
            File *GetFileByName(std::string name);
            Goal *GetGoalByTarget(Target target);
    };
}

#endif