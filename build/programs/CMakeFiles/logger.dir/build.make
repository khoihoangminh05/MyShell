# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 4.0

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

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Projects\MyShell

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Projects\MyShell\build

# Include any dependencies generated for this target.
include programs/CMakeFiles/logger.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include programs/CMakeFiles/logger.dir/compiler_depend.make

# Include the progress variables for this target.
include programs/CMakeFiles/logger.dir/progress.make

# Include the compile flags for this target's objects.
include programs/CMakeFiles/logger.dir/flags.make

programs/CMakeFiles/logger.dir/codegen:
.PHONY : programs/CMakeFiles/logger.dir/codegen

programs/CMakeFiles/logger.dir/logger.cpp.obj: programs/CMakeFiles/logger.dir/flags.make
programs/CMakeFiles/logger.dir/logger.cpp.obj: D:/Projects/MyShell/programs/logger.cpp
programs/CMakeFiles/logger.dir/logger.cpp.obj: programs/CMakeFiles/logger.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\Projects\MyShell\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object programs/CMakeFiles/logger.dir/logger.cpp.obj"
	cd /d D:\Projects\MyShell\build\programs && C:\msys64\ucrt64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT programs/CMakeFiles/logger.dir/logger.cpp.obj -MF CMakeFiles\logger.dir\logger.cpp.obj.d -o CMakeFiles\logger.dir\logger.cpp.obj -c D:\Projects\MyShell\programs\logger.cpp

programs/CMakeFiles/logger.dir/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/logger.dir/logger.cpp.i"
	cd /d D:\Projects\MyShell\build\programs && C:\msys64\ucrt64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Projects\MyShell\programs\logger.cpp > CMakeFiles\logger.dir\logger.cpp.i

programs/CMakeFiles/logger.dir/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/logger.dir/logger.cpp.s"
	cd /d D:\Projects\MyShell\build\programs && C:\msys64\ucrt64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Projects\MyShell\programs\logger.cpp -o CMakeFiles\logger.dir\logger.cpp.s

# Object files for target logger
logger_OBJECTS = \
"CMakeFiles/logger.dir/logger.cpp.obj"

# External object files for target logger
logger_EXTERNAL_OBJECTS =

programs/logger.exe: programs/CMakeFiles/logger.dir/logger.cpp.obj
programs/logger.exe: programs/CMakeFiles/logger.dir/build.make
programs/logger.exe: programs/CMakeFiles/logger.dir/linkLibs.rsp
programs/logger.exe: programs/CMakeFiles/logger.dir/objects1.rsp
programs/logger.exe: programs/CMakeFiles/logger.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\Projects\MyShell\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable logger.exe"
	cd /d D:\Projects\MyShell\build\programs && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\logger.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
programs/CMakeFiles/logger.dir/build: programs/logger.exe
.PHONY : programs/CMakeFiles/logger.dir/build

programs/CMakeFiles/logger.dir/clean:
	cd /d D:\Projects\MyShell\build\programs && $(CMAKE_COMMAND) -P CMakeFiles\logger.dir\cmake_clean.cmake
.PHONY : programs/CMakeFiles/logger.dir/clean

programs/CMakeFiles/logger.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Projects\MyShell D:\Projects\MyShell\programs D:\Projects\MyShell\build D:\Projects\MyShell\build\programs D:\Projects\MyShell\build\programs\CMakeFiles\logger.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : programs/CMakeFiles/logger.dir/depend

