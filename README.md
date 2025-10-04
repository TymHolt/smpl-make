# smpl-make
A simple way to compile and/or build projects. *smpl-make* aims to provide tools to quickly get started and make short, easy to read build scripts. This is a stand alone tool and does not require *GNU Make* to run, just the compilers or build tools you would normally call from the terminal or in *Make*. *Linux* is the only supported platform currently.

## Contents
1. Install / Build from source
2. Usage
3. Makefile.smpl

## Install / Build from source

If you are on a *Debian* based system, head over to the releases of this project. There will be .deb packages ready to install.

 **Build**
 You need the *g++* compiler available on your system. This project can build itself or can build using *Make*. Simply run
 
 > make
 
 or

> smplmake

 to get an executable file, depending which build system you want to use. Of course it is intended that *smpl-make* is used for building itself.

## Usage

Run

> smplmake

to execute the default files (*Makefile.smpl*) main goal. To execute another goal put its name as the first argument. If you put a file name as the argument, it will execute this files main goal.

> smplmake [goal or file name]

To execute a specific goal in a specific file, you can use the following (without the brackets)

> smplmake [file:goal]
 
 You can also execute multiple targets at once if you like

> smplmake [file1:goal1] [file2:goal2] [...]
 
## Makefile.smpl

How to write an example Makefile.smpl


```
# A comment
# $(...) -> smpl-make utility macros
# all-recursive [match] -> All files in directory and sub-directories

# vars are initialized before executing goals
# Macros match using regex
var src_files = $(all-recursive .*\.cpp)

# Main goal is excuted if no goal is specified
goal main {
	# Use vars using %
	g++ -Isrc %(src_files) -o smplmake
}

# This goald must be specified to be executed
goal release {
	g++ -Isrc %(src_files) -o smplmake -mtune=generic -march=x86-64
}

```