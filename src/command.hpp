#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <core_classes.hpp>

namespace smpl {

    class SystemCommand : public ICommand {
        private:
            std::string m_raw_command;
        public:
            SystemCommand(std::string raw_command = "");
            virtual int Execute(ExecutionContext *context);
    };

    class ChangeDirectoryCommand : public ICommand {
        private:
            std::string m_directory_path;
        public:
            ChangeDirectoryCommand(std::string directory_path = "");
            virtual int Execute(ExecutionContext *context);
    };
}

#endif