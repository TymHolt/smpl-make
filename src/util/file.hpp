#ifndef FILE_HPP
#define FILE_HPP

#include <string>

namespace util {

    bool FileExists(std::string file_name);
    std::string LoadTextFile(std::string file_name);
}

#endif