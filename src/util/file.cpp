#include <fstream>
#include <util/file.hpp>

bool util::FileExists(std::string file_name) {
    std::fstream file(file_name);
    bool result = file.is_open();
    file.close();
    return result;    
}