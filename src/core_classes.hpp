#ifndef CORE_CLASSES_HPP
#define CORE_CLASSES_HPP

#include <string>
#include <vector>
#include <file_parsing.hpp>

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
            Variable(std::string name = "", std::string value = "");
            std::string GetName();
            std::string GetValue();
    };

    enum CommandType {
        CT_NONE,
        CT_CHANGE_DIRECTORY,
        CT_SYSTEM
    };

    class Command {
        private:
            CommandType m_type;
            std::string m_value;
        public:
            Command(CommandType type = CT_NONE, std::string value = "");
            CommandType GetType();
            std::string GetValue();
    };

    class Goal {
        private:
            std::string m_name;
            std::vector<Command> m_commands;
        public:
            Goal(std::string name = "", std::vector<Command> commands = {});
            std::string GetName();
            std::vector<Command> GetCommands();
    };

    class File {
        private:
            std::string m_name;
            std::vector<Variable> m_variables;
            std::vector<Goal> m_goals;
        public:
            File(std::string file_name = "", std::vector<Line> lines = {});
            std::string GetName();
            std::vector<Variable> Variables();
            std::vector<Goal> GetGoals();
    };

    class Context {
        private:
            std::vector<Target> m_targets;
            std::vector<File> m_files;

            bool HasFileLoaded(std::string name);
        public:
            Context(std::vector<Target> targets = {});
            bool RunAll();
    };
}

#endif