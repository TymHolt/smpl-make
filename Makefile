cpp_files = ./src/target.cpp ./src/util/file.cpp ./src/util/parse.cpp ./src/macro.cpp ./src/main.cpp ./src/command.cpp ./src/core_classes.cpp 

smplmake: $(cpp_files)
	g++ -Isrc $(cpp_files) -o smplmake

clean:
	rm smplmake
