core_files = src/main.cpp
src_cpp_files = $(core_files)

smplmake: $(src_cpp_files)
	g++ -Isrc $(src_cpp_files) -o smplmake

clean:
	rm smplmake