# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/intelligentrobotics/rokon_clone/October/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/intelligentrobotics/rokon_clone/October/build

# Include any dependencies generated for this target.
include unit/CMakeFiles/vel.dir/depend.make

# Include the progress variables for this target.
include unit/CMakeFiles/vel.dir/progress.make

# Include the compile flags for this target's objects.
include unit/CMakeFiles/vel.dir/flags.make

unit/CMakeFiles/vel.dir/vel.cpp.o: unit/CMakeFiles/vel.dir/flags.make
unit/CMakeFiles/vel.dir/vel.cpp.o: /home/intelligentrobotics/rokon_clone/October/src/unit/vel.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/intelligentrobotics/rokon_clone/October/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object unit/CMakeFiles/vel.dir/vel.cpp.o"
	cd /home/intelligentrobotics/rokon_clone/October/build/unit && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vel.dir/vel.cpp.o -c /home/intelligentrobotics/rokon_clone/October/src/unit/vel.cpp

unit/CMakeFiles/vel.dir/vel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vel.dir/vel.cpp.i"
	cd /home/intelligentrobotics/rokon_clone/October/build/unit && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/intelligentrobotics/rokon_clone/October/src/unit/vel.cpp > CMakeFiles/vel.dir/vel.cpp.i

unit/CMakeFiles/vel.dir/vel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vel.dir/vel.cpp.s"
	cd /home/intelligentrobotics/rokon_clone/October/build/unit && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/intelligentrobotics/rokon_clone/October/src/unit/vel.cpp -o CMakeFiles/vel.dir/vel.cpp.s

unit/CMakeFiles/vel.dir/vel.cpp.o.requires:
.PHONY : unit/CMakeFiles/vel.dir/vel.cpp.o.requires

unit/CMakeFiles/vel.dir/vel.cpp.o.provides: unit/CMakeFiles/vel.dir/vel.cpp.o.requires
	$(MAKE) -f unit/CMakeFiles/vel.dir/build.make unit/CMakeFiles/vel.dir/vel.cpp.o.provides.build
.PHONY : unit/CMakeFiles/vel.dir/vel.cpp.o.provides

unit/CMakeFiles/vel.dir/vel.cpp.o.provides.build: unit/CMakeFiles/vel.dir/vel.cpp.o

# Object files for target vel
vel_OBJECTS = \
"CMakeFiles/vel.dir/vel.cpp.o"

# External object files for target vel
vel_EXTERNAL_OBJECTS =

/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: unit/CMakeFiles/vel.dir/vel.cpp.o
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /opt/ros/groovy/lib/libroscpp.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/libboost_signals-mt.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/libboost_filesystem-mt.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /opt/ros/groovy/lib/librosconsole.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/libboost_regex-mt.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/liblog4cxx.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /opt/ros/groovy/lib/libroscpp_serialization.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /opt/ros/groovy/lib/librostime.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/libboost_date_time-mt.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/libboost_system-mt.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/libboost_thread-mt.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /opt/ros/groovy/lib/libxmlrpcpp.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /opt/ros/groovy/lib/libcpp_common.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /home/intelligentrobotics/rokon_clone/October/devel/lib/libutils.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /home/intelligentrobotics/rokon_clone/October/devel/lib/libmain.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /home/intelligentrobotics/rokon_clone/October/devel/lib/libutils.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /opt/ros/groovy/lib/libroscpp.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/libboost_signals-mt.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/libboost_filesystem-mt.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /opt/ros/groovy/lib/librosconsole.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/libboost_regex-mt.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/liblog4cxx.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /opt/ros/groovy/lib/libroscpp_serialization.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /opt/ros/groovy/lib/librostime.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/libboost_date_time-mt.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/libboost_system-mt.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/libboost_thread-mt.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /opt/ros/groovy/lib/libxmlrpcpp.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: /opt/ros/groovy/lib/libcpp_common.so
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: unit/CMakeFiles/vel.dir/build.make
/home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel: unit/CMakeFiles/vel.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable /home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel"
	cd /home/intelligentrobotics/rokon_clone/October/build/unit && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vel.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
unit/CMakeFiles/vel.dir/build: /home/intelligentrobotics/rokon_clone/October/devel/lib/unit/vel
.PHONY : unit/CMakeFiles/vel.dir/build

unit/CMakeFiles/vel.dir/requires: unit/CMakeFiles/vel.dir/vel.cpp.o.requires
.PHONY : unit/CMakeFiles/vel.dir/requires

unit/CMakeFiles/vel.dir/clean:
	cd /home/intelligentrobotics/rokon_clone/October/build/unit && $(CMAKE_COMMAND) -P CMakeFiles/vel.dir/cmake_clean.cmake
.PHONY : unit/CMakeFiles/vel.dir/clean

unit/CMakeFiles/vel.dir/depend:
	cd /home/intelligentrobotics/rokon_clone/October/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/intelligentrobotics/rokon_clone/October/src /home/intelligentrobotics/rokon_clone/October/src/unit /home/intelligentrobotics/rokon_clone/October/build /home/intelligentrobotics/rokon_clone/October/build/unit /home/intelligentrobotics/rokon_clone/October/build/unit/CMakeFiles/vel.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : unit/CMakeFiles/vel.dir/depend

