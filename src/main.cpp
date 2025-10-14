#include <iostream>
#include <vector>
#include <core_classes.hpp>
#include <target.hpp>

std::vector<smpl::Target> ParseTargetsFromArguments(int argc, char **argv) {
    std::vector<smpl::Target> targets;

    // No arguments given, default target
    if (argc <= 1) {
        smpl::Target target;
        targets.push_back(target);
        return targets;
    }

    for (int index = 1; index < argc; index++) {
        const char *argument = argv[index];
        smpl::Target target(argument);
        targets.push_back(target);
    }

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