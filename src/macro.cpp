#include <macro.hpp>
#include <util/string_parsing.hpp>
#include <stdexcept>
#include <filesystem>
#include <regex>

bool EndsWith(std::string string, std::string suffix) {
    return string.size() >= suffix.size() && 
        string.compare(string.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool MatchPath(std::string path, std::string argument) {
    std::regex expression(argument);
    return std::regex_match(path, expression);
}

std::string RunMacroAllRecursive(std::string argument) {
    std::string result = "";

    for (const auto& entry : std::filesystem::recursive_directory_iterator(".")) {
        if (entry.is_regular_file() && MatchPath(entry.path(), argument))
            result += std::string(entry.path()) + std::string(" ");
    }

    return result;
}

std::string Macro::RunMacro(std::string macro) {
    util::StringParser string_parser;
    string_parser.SetContent(macro);

    std::string macro_name = string_parser.NextToken();
    std::string macro_argument = string_parser.GetRemaining();
    
    if (macro_name == "all-recursive")
        return RunMacroAllRecursive(macro_argument);
    else
        throw std::runtime_error(std::string("Unknown macro " + macro_name));
}