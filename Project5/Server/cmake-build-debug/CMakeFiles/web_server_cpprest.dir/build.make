# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/ningsong/CLionProjects/Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/ningsong/CLionProjects/Server/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/web_server_cpprest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/web_server_cpprest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/web_server_cpprest.dir/flags.make

CMakeFiles/web_server_cpprest.dir/main.cpp.o: CMakeFiles/web_server_cpprest.dir/flags.make
CMakeFiles/web_server_cpprest.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/ningsong/CLionProjects/Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/web_server_cpprest.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/web_server_cpprest.dir/main.cpp.o -c /Users/ningsong/CLionProjects/Server/main.cpp

CMakeFiles/web_server_cpprest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/web_server_cpprest.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/ningsong/CLionProjects/Server/main.cpp > CMakeFiles/web_server_cpprest.dir/main.cpp.i

CMakeFiles/web_server_cpprest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/web_server_cpprest.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/ningsong/CLionProjects/Server/main.cpp -o CMakeFiles/web_server_cpprest.dir/main.cpp.s

# Object files for target web_server_cpprest
web_server_cpprest_OBJECTS = \
"CMakeFiles/web_server_cpprest.dir/main.cpp.o"

# External object files for target web_server_cpprest
web_server_cpprest_EXTERNAL_OBJECTS =

web_server_cpprest: CMakeFiles/web_server_cpprest.dir/main.cpp.o
web_server_cpprest: CMakeFiles/web_server_cpprest.dir/build.make
web_server_cpprest: /usr/local/lib/libboost_system-mt.dylib
web_server_cpprest: /usr/local/lib/libboost_thread-mt.dylib
web_server_cpprest: /usr/local/lib/libboost_log-mt.dylib
web_server_cpprest: /usr/local/lib/libboost_program_options-mt.dylib
web_server_cpprest: /usr/local/lib/libboost_chrono-mt.dylib
web_server_cpprest: /usr/local/lib/libboost_date_time-mt.dylib
web_server_cpprest: /usr/local/lib/libboost_atomic-mt.dylib
web_server_cpprest: /usr/local/lib/libboost_log_setup-mt.dylib
web_server_cpprest: /usr/local/lib/libboost_filesystem-mt.dylib
web_server_cpprest: /usr/local/lib/libboost_regex-mt.dylib
web_server_cpprest: /usr/lib/libcrypto.dylib
web_server_cpprest: /usr/lib/libssl.dylib
web_server_cpprest: CMakeFiles/web_server_cpprest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/ningsong/CLionProjects/Server/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable web_server_cpprest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/web_server_cpprest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/web_server_cpprest.dir/build: web_server_cpprest

.PHONY : CMakeFiles/web_server_cpprest.dir/build

CMakeFiles/web_server_cpprest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/web_server_cpprest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/web_server_cpprest.dir/clean

CMakeFiles/web_server_cpprest.dir/depend:
	cd /Users/ningsong/CLionProjects/Server/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/ningsong/CLionProjects/Server /Users/ningsong/CLionProjects/Server /Users/ningsong/CLionProjects/Server/cmake-build-debug /Users/ningsong/CLionProjects/Server/cmake-build-debug /Users/ningsong/CLionProjects/Server/cmake-build-debug/CMakeFiles/web_server_cpprest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/web_server_cpprest.dir/depend
