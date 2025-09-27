#include <iostream>
#include <string>
#include <string.h>
#include <fstream>

typedef struct Target_Struct {
    std::string file_name;
    std::string goal_name;
} Target;

void AssignDefaultTarget(Target *target) {
    target->file_name = "Makefile.smpl";
    target->goal_name = "main";
}

void ParseTarget(Target *target, char* argument) {
    std::string token_left = "";
    size_t index = 0;
    size_t length = strlen(argument);

    for (; index < length; index++) {
        char current_char = argument[index];
    
        if (current_char == ':') {
            index++;
            break;
        }

        token_left += current_char;
    }

    // TODO: Search if file name exists first, otherwise use as goal name
    if (index >= length) {
        target->goal_name = token_left;
        return;
    }

    std::string token_right = "";

    for (; index < length; index++) {
        char current_char = argument[index];
        token_right += current_char;
    }

    target->file_name = token_left;
    target->goal_name = token_right;
}

bool ExecuteTarget(Target *target) {
    std::cout << "Running goal " << target->file_name << ":" << target->goal_name << std::endl;

    std::fstream file(target->file_name);

    if (!file.is_open()) {
        std::cout << "Unable to open file " << target->file_name << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Process line
        std::cout << line << std::endl;
    }

    file.close();
    return true;
}

int main(int argc, char** argv) {
    Target target;
    
    if (argc < 2) {
        AssignDefaultTarget(&target);
        return !ExecuteTarget(&target);
    }

    bool result = true;
    for (int arg_index = 1; arg_index < argc; arg_index++) {
        AssignDefaultTarget(&target);
        ParseTarget(&target, argv[arg_index]);
        result = ExecuteTarget(&target) && result;
    }

    return !result;
}