# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.26.4/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.26.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kiran/Documents/Projects/ICLR/librnp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kiran/Documents/Projects/ICLR/librnp/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/messagepacket_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/messagepacket_test.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/messagepacket_test.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/messagepacket_test.dir/flags.make

tests/CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.o: tests/CMakeFiles/messagepacket_test.dir/flags.make
tests/CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.o: /Users/kiran/Documents/Projects/ICLR/librnp/tests/messagepacket_test.cpp
tests/CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.o: tests/CMakeFiles/messagepacket_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kiran/Documents/Projects/ICLR/librnp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.o"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.o -MF CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.o.d -o CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.o -c /Users/kiran/Documents/Projects/ICLR/librnp/tests/messagepacket_test.cpp

tests/CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.i"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kiran/Documents/Projects/ICLR/librnp/tests/messagepacket_test.cpp > CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.i

tests/CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.s"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kiran/Documents/Projects/ICLR/librnp/tests/messagepacket_test.cpp -o CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.s

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.o: tests/CMakeFiles/messagepacket_test.dir/flags.make
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.o: /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/default_packets/simplecommandpacket.cpp
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.o: tests/CMakeFiles/messagepacket_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kiran/Documents/Projects/ICLR/librnp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.o"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.o -MF CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.o.d -o CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.o -c /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/default_packets/simplecommandpacket.cpp

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.i"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/default_packets/simplecommandpacket.cpp > CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.i

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.s"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/default_packets/simplecommandpacket.cpp -o CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.s

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.o: tests/CMakeFiles/messagepacket_test.dir/flags.make
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.o: /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/loopback.cpp
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.o: tests/CMakeFiles/messagepacket_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kiran/Documents/Projects/ICLR/librnp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.o"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.o -MF CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.o.d -o CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.o -c /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/loopback.cpp

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.i"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/loopback.cpp > CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.i

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.s"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/loopback.cpp -o CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.s

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.o: tests/CMakeFiles/messagepacket_test.dir/flags.make
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.o: /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/printer.cpp
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.o: tests/CMakeFiles/messagepacket_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kiran/Documents/Projects/ICLR/librnp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.o"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.o -MF CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.o.d -o CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.o -c /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/printer.cpp

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.i"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/printer.cpp > CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.i

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.s"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/printer.cpp -o CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.s

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.o: tests/CMakeFiles/messagepacket_test.dir/flags.make
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.o: /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_header.cpp
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.o: tests/CMakeFiles/messagepacket_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kiran/Documents/Projects/ICLR/librnp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.o"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.o -MF CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.o.d -o CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.o -c /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_header.cpp

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.i"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_header.cpp > CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.i

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.s"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_header.cpp -o CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.s

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.o: tests/CMakeFiles/messagepacket_test.dir/flags.make
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.o: /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_netman_packets.cpp
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.o: tests/CMakeFiles/messagepacket_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kiran/Documents/Projects/ICLR/librnp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.o"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.o -MF CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.o.d -o CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.o -c /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_netman_packets.cpp

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.i"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_netman_packets.cpp > CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.i

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.s"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_netman_packets.cpp -o CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.s

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.o: tests/CMakeFiles/messagepacket_test.dir/flags.make
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.o: /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_networkmanager.cpp
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.o: tests/CMakeFiles/messagepacket_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kiran/Documents/Projects/ICLR/librnp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.o"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.o -MF CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.o.d -o CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.o -c /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_networkmanager.cpp

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.i"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_networkmanager.cpp > CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.i

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.s"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_networkmanager.cpp -o CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.s

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.o: tests/CMakeFiles/messagepacket_test.dir/flags.make
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.o: /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_packet.cpp
tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.o: tests/CMakeFiles/messagepacket_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/kiran/Documents/Projects/ICLR/librnp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.o"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.o -MF CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.o.d -o CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.o -c /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_packet.cpp

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.i"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_packet.cpp > CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.i

tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.s"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kiran/Documents/Projects/ICLR/librnp/src/librnp/rnp_packet.cpp -o CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.s

# Object files for target messagepacket_test
messagepacket_test_OBJECTS = \
"CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.o" \
"CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.o" \
"CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.o" \
"CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.o" \
"CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.o" \
"CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.o" \
"CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.o" \
"CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.o"

# External object files for target messagepacket_test
messagepacket_test_EXTERNAL_OBJECTS =

/Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test: tests/CMakeFiles/messagepacket_test.dir/messagepacket_test.cpp.o
/Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test: tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/default_packets/simplecommandpacket.cpp.o
/Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test: tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/loopback.cpp.o
/Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test: tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/printer.cpp.o
/Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test: tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_header.cpp.o
/Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test: tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_netman_packets.cpp.o
/Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test: tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_networkmanager.cpp.o
/Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test: tests/CMakeFiles/messagepacket_test.dir/__/src/librnp/rnp_packet.cpp.o
/Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test: tests/CMakeFiles/messagepacket_test.dir/build.make
/Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test: /Users/kiran/Documents/Projects/ICLR/librnp/lib/liblibrnp.a
/Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test: tests/CMakeFiles/messagepacket_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/kiran/Documents/Projects/ICLR/librnp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable /Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test"
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/messagepacket_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/messagepacket_test.dir/build: /Users/kiran/Documents/Projects/ICLR/librnp/bin/messagepacket_test
.PHONY : tests/CMakeFiles/messagepacket_test.dir/build

tests/CMakeFiles/messagepacket_test.dir/clean:
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/messagepacket_test.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/messagepacket_test.dir/clean

tests/CMakeFiles/messagepacket_test.dir/depend:
	cd /Users/kiran/Documents/Projects/ICLR/librnp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kiran/Documents/Projects/ICLR/librnp /Users/kiran/Documents/Projects/ICLR/librnp/tests /Users/kiran/Documents/Projects/ICLR/librnp/build /Users/kiran/Documents/Projects/ICLR/librnp/build/tests /Users/kiran/Documents/Projects/ICLR/librnp/build/tests/CMakeFiles/messagepacket_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/messagepacket_test.dir/depend
