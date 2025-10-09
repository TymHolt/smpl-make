#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

namespace smpl {

    typedef struct VariableStruct {
        std::string m_name;
        std::string m_value;
    } Variable;

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
            virtual int Execute(ExecutionContext* context);
    };

    class SystemCommand : public ICommand {
        private:
            std::string m_raw_command;
        public:
            SystemCommand(std::string raw_command = "");
            virtual int Execute(ExecutionContext* context);
    };
}

#endif