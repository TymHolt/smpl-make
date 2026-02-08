#include <iostream>
#include <vector>
#include <core_classes.hpp>
#include <file_loading.hpp>

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

bool Run(int argc, char **argv) {
    bool result = true;

    smpl::Context context;
    std::vector<smpl::Target> targets = ParseTargetsFromArguments(argc, argv);
    for (smpl::Target target : targets) {
        // Load file if ot already loaded
        if (context.GetFileByName(target.GetFileName()) == NULL) {
            if (!smpl::LoadFile(context.AddFile(target.GetFileName()))) {
                std::cerr << "Could not load file " << target.GetFileName() << std::endl;
                return false;
            }
        }

        // Find goal for target
        smpl::Goal *goal = context.GetGoalByTarget(target);
        if (goal == NULL) {
            std::cerr << "Could not find goal " << target.GetFileName() << ":" << target.GetGoalName() << std::endl;
            return false;
        }

        // TODO Execute goal with Runtime
    }

    return true;
}

int main(int argc, char **argv) {
    try {
        if (Run(argc, argv))
            return EXIT_SUCCESS;
    } catch (const std::runtime_error& exception) {
        std::cerr << "(Internal) Error: " << exception.what() << std::endl;
    } catch (...) {
        std::cerr << "(Internal) Uknown exception" << std::endl;
    }

    return EXIT_FAILURE;
}