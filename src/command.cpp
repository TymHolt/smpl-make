#include <command.hpp>
#include <iostream>

smpl::SystemCommand::SystemCommand(std::string raw_command) {
    m_raw_command = raw_command;
}

int smpl::SystemCommand::Execute(ExecutionContext *context) {
    std::string command = m_raw_command;
    command = context->ApplyVariables(command);
    std::string execution_directory = context->GetExecutionDirectory();

    // Call to cd in same context if execution directory is defined
    if (execution_directory != "")
        command = "cd " + execution_directory + " && " + command;

    std::cout << command << std::endl;
    return system(command.c_str());
}

smpl::ChangeDirectoryCommand::ChangeDirectoryCommand(std::string directory_path) {
    m_directory_path = directory_path;
}

int smpl::ChangeDirectoryCommand::Execute(ExecutionContext *context) {
    context->SetExecutionDirectory(m_directory_path);
    return 0;
}