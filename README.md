# smpl-make

A simple way to build projects

Build:
> make

### Usage (WIP)

> ./smplmake

This will execute the main goal located in the default file (Makefile.smpl)

> ./smplmake argument

This will either:
1. Execute the main goal of the file specified by the argument
2. If no file with that name exists, the default file will be used and the argument specifies the goal name

> ./smplmake file:goal

This will execute the given goal in the given file