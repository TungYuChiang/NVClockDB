# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/oslab/Desktop/TungYu/leveldb

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/oslab/Desktop/TungYu/leveldb/build

# Include any dependencies generated for this target.
include third_party/benchmark/src/CMakeFiles/benchmark_main.dir/depend.make

# Include the progress variables for this target.
include third_party/benchmark/src/CMakeFiles/benchmark_main.dir/progress.make

# Include the compile flags for this target's objects.
include third_party/benchmark/src/CMakeFiles/benchmark_main.dir/flags.make

third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o: third_party/benchmark/src/CMakeFiles/benchmark_main.dir/flags.make
third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o: ../third_party/benchmark/src/benchmark_main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/oslab/Desktop/TungYu/leveldb/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o"
	cd /home/oslab/Desktop/TungYu/leveldb/build/third_party/benchmark/src && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/benchmark_main.dir/benchmark_main.cc.o -c /home/oslab/Desktop/TungYu/leveldb/third_party/benchmark/src/benchmark_main.cc

third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/benchmark_main.dir/benchmark_main.cc.i"
	cd /home/oslab/Desktop/TungYu/leveldb/build/third_party/benchmark/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/oslab/Desktop/TungYu/leveldb/third_party/benchmark/src/benchmark_main.cc > CMakeFiles/benchmark_main.dir/benchmark_main.cc.i

third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/benchmark_main.dir/benchmark_main.cc.s"
	cd /home/oslab/Desktop/TungYu/leveldb/build/third_party/benchmark/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/oslab/Desktop/TungYu/leveldb/third_party/benchmark/src/benchmark_main.cc -o CMakeFiles/benchmark_main.dir/benchmark_main.cc.s

third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o.requires:

.PHONY : third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o.requires

third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o.provides: third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o.requires
	$(MAKE) -f third_party/benchmark/src/CMakeFiles/benchmark_main.dir/build.make third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o.provides.build
.PHONY : third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o.provides

third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o.provides.build: third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o


# Object files for target benchmark_main
benchmark_main_OBJECTS = \
"CMakeFiles/benchmark_main.dir/benchmark_main.cc.o"

# External object files for target benchmark_main
benchmark_main_EXTERNAL_OBJECTS =

third_party/benchmark/src/libbenchmark_main.a: third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o
third_party/benchmark/src/libbenchmark_main.a: third_party/benchmark/src/CMakeFiles/benchmark_main.dir/build.make
third_party/benchmark/src/libbenchmark_main.a: third_party/benchmark/src/CMakeFiles/benchmark_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/oslab/Desktop/TungYu/leveldb/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libbenchmark_main.a"
	cd /home/oslab/Desktop/TungYu/leveldb/build/third_party/benchmark/src && $(CMAKE_COMMAND) -P CMakeFiles/benchmark_main.dir/cmake_clean_target.cmake
	cd /home/oslab/Desktop/TungYu/leveldb/build/third_party/benchmark/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/benchmark_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
third_party/benchmark/src/CMakeFiles/benchmark_main.dir/build: third_party/benchmark/src/libbenchmark_main.a

.PHONY : third_party/benchmark/src/CMakeFiles/benchmark_main.dir/build

third_party/benchmark/src/CMakeFiles/benchmark_main.dir/requires: third_party/benchmark/src/CMakeFiles/benchmark_main.dir/benchmark_main.cc.o.requires

.PHONY : third_party/benchmark/src/CMakeFiles/benchmark_main.dir/requires

third_party/benchmark/src/CMakeFiles/benchmark_main.dir/clean:
	cd /home/oslab/Desktop/TungYu/leveldb/build/third_party/benchmark/src && $(CMAKE_COMMAND) -P CMakeFiles/benchmark_main.dir/cmake_clean.cmake
.PHONY : third_party/benchmark/src/CMakeFiles/benchmark_main.dir/clean

third_party/benchmark/src/CMakeFiles/benchmark_main.dir/depend:
	cd /home/oslab/Desktop/TungYu/leveldb/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/oslab/Desktop/TungYu/leveldb /home/oslab/Desktop/TungYu/leveldb/third_party/benchmark/src /home/oslab/Desktop/TungYu/leveldb/build /home/oslab/Desktop/TungYu/leveldb/build/third_party/benchmark/src /home/oslab/Desktop/TungYu/leveldb/build/third_party/benchmark/src/CMakeFiles/benchmark_main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : third_party/benchmark/src/CMakeFiles/benchmark_main.dir/depend

