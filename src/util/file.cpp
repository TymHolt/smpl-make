#include <fstream>
#include <stdexcept>
#include <util/file.hpp>

bool util::FileExists(std::string file_name) {
    std::fstream file_stream(file_name);
    bool result = file_stream.is_open();
    file_stream.close();
    return result;    
}

std::string util::LoadTextFile(std::string file_name) {
    std::fstream file_stream(file_name);

    if (!file_stream.is_open())
        throw std::runtime_error(std::string("Failed to open file " + file_name));

    std::string result = "";

    std::string line;
    while (std::getline(file_stream, line))
        result += line + '\n';
    
    file_stream.close();
    return result;  
}