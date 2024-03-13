# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/d/Universidad/tfg/engine/Reveal3d

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/Universidad/tfg/engine/Reveal3d/cmake-build-relwithdebinfo

# Include any dependencies generated for this target.
include Sample/CMakeFiles/Sample.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Sample/CMakeFiles/Sample.dir/compiler_depend.make

# Include the progress variables for this target.
include Sample/CMakeFiles/Sample.dir/progress.make

# Include the compile flags for this target's objects.
include Sample/CMakeFiles/Sample.dir/flags.make

Sample/CMakeFiles/Sample.dir/main.cpp.o: Sample/CMakeFiles/Sample.dir/flags.make
Sample/CMakeFiles/Sample.dir/main.cpp.o: ../Sample/main.cpp
Sample/CMakeFiles/Sample.dir/main.cpp.o: Sample/CMakeFiles/Sample.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/Universidad/tfg/engine/Reveal3d/cmake-build-relwithdebinfo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Sample/CMakeFiles/Sample.dir/main.cpp.o"
	cd /mnt/d/Universidad/tfg/engine/Reveal3d/cmake-build-relwithdebinfo/Sample && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Sample/CMakeFiles/Sample.dir/main.cpp.o -MF CMakeFiles/Sample.dir/main.cpp.o.d -o CMakeFiles/Sample.dir/main.cpp.o -c /mnt/d/Universidad/tfg/engine/Reveal3d/Sample/main.cpp

Sample/CMakeFiles/Sample.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Sample.dir/main.cpp.i"
	cd /mnt/d/Universidad/tfg/engine/Reveal3d/cmake-build-relwithdebinfo/Sample && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/Universidad/tfg/engine/Reveal3d/Sample/main.cpp > CMakeFiles/Sample.dir/main.cpp.i

Sample/CMakeFiles/Sample.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Sample.dir/main.cpp.s"
	cd /mnt/d/Universidad/tfg/engine/Reveal3d/cmake-build-relwithdebinfo/Sample && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/Universidad/tfg/engine/Reveal3d/Sample/main.cpp -o CMakeFiles/Sample.dir/main.cpp.s

# Object files for target Sample
Sample_OBJECTS = \
"CMakeFiles/Sample.dir/main.cpp.o"

# External object files for target Sample
Sample_EXTERNAL_OBJECTS =

Sample/Sample: Sample/CMakeFiles/Sample.dir/main.cpp.o
Sample/Sample: Sample/CMakeFiles/Sample.dir/build.make
Sample/Sample: Engine/libEngine.a
Sample/Sample: Sample/CMakeFiles/Sample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/Universidad/tfg/engine/Reveal3d/cmake-build-relwithdebinfo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Sample"
	cd /mnt/d/Universidad/tfg/engine/Reveal3d/cmake-build-relwithdebinfo/Sample && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Sample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Sample/CMakeFiles/Sample.dir/build: Sample/Sample
.PHONY : Sample/CMakeFiles/Sample.dir/build

Sample/CMakeFiles/Sample.dir/clean:
	cd /mnt/d/Universidad/tfg/engine/Reveal3d/cmake-build-relwithdebinfo/Sample && $(CMAKE_COMMAND) -P CMakeFiles/Sample.dir/cmake_clean.cmake
.PHONY : Sample/CMakeFiles/Sample.dir/clean

Sample/CMakeFiles/Sample.dir/depend:
	cd /mnt/d/Universidad/tfg/engine/Reveal3d/cmake-build-relwithdebinfo && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/Universidad/tfg/engine/Reveal3d /mnt/d/Universidad/tfg/engine/Reveal3d/Sample /mnt/d/Universidad/tfg/engine/Reveal3d/cmake-build-relwithdebinfo /mnt/d/Universidad/tfg/engine/Reveal3d/cmake-build-relwithdebinfo/Sample /mnt/d/Universidad/tfg/engine/Reveal3d/cmake-build-relwithdebinfo/Sample/CMakeFiles/Sample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Sample/CMakeFiles/Sample.dir/depend

