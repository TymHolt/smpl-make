core_files = src/main.cpp src/target.cpp src/parse.cpp src/macro.cpp
src_cpp_files = $(core_files)

smplmake: $(src_cpp_files)
	g++ -Isrc $(src_cpp_files) -o smplmake

clean:
	rm smplmake