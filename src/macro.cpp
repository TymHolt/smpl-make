#include <macro.hpp>
#include <filesystem>
#include <regex>

smpl::AllRecursiveMacro::AllRecursiveMacro(std::string regex_string) {
    m_regex_string = regex_string;
}

bool MatchFilePath(std::string path, std::string regex_string) {
    std::regex regex_expression(regex_string);
    return std::regex_match(path, regex_expression);
}

std::string smpl::AllRecursiveMacro::Run() {
    std::string result = "";

    for (const auto& entry : std::filesystem::recursive_directory_iterator("."))
        if (entry.is_regular_file() && MatchFilePath(entry.path(), m_regex_string))
            result += std::string(entry.path()) + std::string(" ");

    return result;
};