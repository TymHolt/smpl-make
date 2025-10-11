#include <iostream>
#include <vector>
#include <core_classes.hpp>
#include <util/parse.hpp>
#include <util/file.hpp>
#include <target.hpp>

#define DEFAULT_FILE_NAME "Makefile.smpl"
#define DEFAULT_GOAL_NAME "main"

smpl::Target ParseTargetFromArgument(char *argument) {
    std::string part_left;
    std::string part_right;
    util::SplitAtChar(std::string(argument), ':', &part_left, &part_right);

    // Default values
    std::string file_name = DEFAULT_FILE_NAME;
    std::string goal_name = DEFAULT_GOAL_NAME;

    // Only one name given, test if file or goal name is meant
    if (part_right == "") {
        if (util::FileExists(part_left))
            file_name = part_left;
        else
            goal_name = part_left;
    } else {
        file_name = part_left;
        goal_name = part_right;
    }

    smpl::Target target(file_name, goal_name);
    return target;
}

std::vector<smpl::Target> ParseTargetsFromArguments(int argc, char **argv) {
    std::vector<smpl::Target> targets;

    // No arguments given, default target
    if (argc <= 1) {
        smpl::Target defualt_target(DEFAULT_FILE_NAME, DEFAULT_GOAL_NAME);
        targets.push_back(defualt_target);
        return targets;
    }

    for (int index = 1; index < argc; index++)
        targets.push_back(ParseTargetFromArgument(argv[index]));
    
    return targets;
}

int Run(int argc, char **argv) {
    bool result = true;

    std::vector<smpl::Target> targets = ParseTargetsFromArguments(argc, argv);
    for (smpl::Target target : targets) {
        SmplTarget legacy_target(target.GetFileName(), target.GetGoalName());
        result = result && legacy_target.Run();
    }

    return result ? 0 : -1;
}

int main(int argc, char **argv) {
    try {
        return Run(argc, argv);
    } catch (const std::runtime_error& exception) {
        std::cerr << "(Internal) Error: " << exception.what() << std::endl;
    } catch (...) {
        std::cerr << "(Internal) Uknown exception" << std::endl;
    }

    return -1;
}