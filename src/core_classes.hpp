#ifndef CORE_CLASSES_HPP
#define CORE_CLASSES_HPP

#include <string>
#include <vector>

namespace smpl {

    class ICommand;

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

    class Goal {
        private:
            std::string m_name;
            std::vector<smpl::ICommand *> m_commands;
        public:
            Goal(std::string name = "", std::vector<smpl::ICommand *> commands = {});
            std::string GetName();
            std::vector<smpl::ICommand *> GetCommands();
    };

    class File {
        private:
            std::string m_name;
            std::vector<Variable> m_variables;
            std::vector<Goal> m_goals;
        public:
            File(std::string file_name);
            std::string GetName();
            std::vector<Variable> Variables();
            std::vector<Goal> GetGoals();
    };

    class ExecutionContext {
        private:
            std::string m_execution_directory;
            std::vector<Variable> m_variables;
        public:
            ExecutionContext();
            void SetExecutionDirectory(std::string execution_directory);
            std::string GetExecutionDirectory();
            void AddVariable(std::string name, std::string value);
            bool ContainsVariable(std::string name);
            std::string ApplyVariables(std::string string);
    };

    class ICommand {
        public:
            ICommand();
            virtual int Execute(ExecutionContext *context);
    };

    class Context {
        private:
            std::vector<File> m_files;
        public:
            Context(std::vector<Target> targets = {});
    };
}

#endif