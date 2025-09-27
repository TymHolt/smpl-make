#include <iostream>
#include <target.hpp>

int main(int argc, char** argv) {
    if (argc <= 1) {
        SmplTarget target;
        return !target.Run();
    }

    bool result = true;
    for (int arg_index = 1; arg_index < argc; arg_index++) {
        SmplTarget target;
        target.ParseArgument(argv[arg_index]);
        result = target.Run() && result;
    }

    return !result;
}