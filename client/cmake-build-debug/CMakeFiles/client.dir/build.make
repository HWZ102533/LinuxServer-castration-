# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /home/admin/clion-2021.1.3/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/admin/clion-2021.1.3/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/admin/CLionProjects/client-new

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/admin/CLionProjects/client-new/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/main.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/admin/CLionProjects/client-new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client.dir/main.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/main.cpp.o -c /home/admin/CLionProjects/client-new/main.cpp

CMakeFiles/client.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/main.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/admin/CLionProjects/client-new/main.cpp > CMakeFiles/client.dir/main.cpp.i

CMakeFiles/client.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/main.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/admin/CLionProjects/client-new/main.cpp -o CMakeFiles/client.dir/main.cpp.s

CMakeFiles/client.dir/CSocket.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/CSocket.cpp.o: ../CSocket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/admin/CLionProjects/client-new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/client.dir/CSocket.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/CSocket.cpp.o -c /home/admin/CLionProjects/client-new/CSocket.cpp

CMakeFiles/client.dir/CSocket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/CSocket.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/admin/CLionProjects/client-new/CSocket.cpp > CMakeFiles/client.dir/CSocket.cpp.i

CMakeFiles/client.dir/CSocket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/CSocket.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/admin/CLionProjects/client-new/CSocket.cpp -o CMakeFiles/client.dir/CSocket.cpp.s

CMakeFiles/client.dir/CCrypto.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/CCrypto.cpp.o: ../CCrypto.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/admin/CLionProjects/client-new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/client.dir/CCrypto.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/CCrypto.cpp.o -c /home/admin/CLionProjects/client-new/CCrypto.cpp

CMakeFiles/client.dir/CCrypto.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/CCrypto.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/admin/CLionProjects/client-new/CCrypto.cpp > CMakeFiles/client.dir/CCrypto.cpp.i

CMakeFiles/client.dir/CCrypto.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/CCrypto.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/admin/CLionProjects/client-new/CCrypto.cpp -o CMakeFiles/client.dir/CCrypto.cpp.s

CMakeFiles/client.dir/CCryptoAES.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/CCryptoAES.cpp.o: ../CCryptoAES.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/admin/CLionProjects/client-new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/client.dir/CCryptoAES.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/CCryptoAES.cpp.o -c /home/admin/CLionProjects/client-new/CCryptoAES.cpp

CMakeFiles/client.dir/CCryptoAES.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/CCryptoAES.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/admin/CLionProjects/client-new/CCryptoAES.cpp > CMakeFiles/client.dir/CCryptoAES.cpp.i

CMakeFiles/client.dir/CCryptoAES.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/CCryptoAES.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/admin/CLionProjects/client-new/CCryptoAES.cpp -o CMakeFiles/client.dir/CCryptoAES.cpp.s

CMakeFiles/client.dir/CProtocol.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/CProtocol.cpp.o: ../CProtocol.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/admin/CLionProjects/client-new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/client.dir/CProtocol.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/CProtocol.cpp.o -c /home/admin/CLionProjects/client-new/CProtocol.cpp

CMakeFiles/client.dir/CProtocol.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/CProtocol.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/admin/CLionProjects/client-new/CProtocol.cpp > CMakeFiles/client.dir/CProtocol.cpp.i

CMakeFiles/client.dir/CProtocol.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/CProtocol.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/admin/CLionProjects/client-new/CProtocol.cpp -o CMakeFiles/client.dir/CProtocol.cpp.s

CMakeFiles/client.dir/CProtocolHttp.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/CProtocolHttp.cpp.o: ../CProtocolHttp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/admin/CLionProjects/client-new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/client.dir/CProtocolHttp.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/CProtocolHttp.cpp.o -c /home/admin/CLionProjects/client-new/CProtocolHttp.cpp

CMakeFiles/client.dir/CProtocolHttp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/CProtocolHttp.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/admin/CLionProjects/client-new/CProtocolHttp.cpp > CMakeFiles/client.dir/CProtocolHttp.cpp.i

CMakeFiles/client.dir/CProtocolHttp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/CProtocolHttp.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/admin/CLionProjects/client-new/CProtocolHttp.cpp -o CMakeFiles/client.dir/CProtocolHttp.cpp.s

CMakeFiles/client.dir/CTimeWheel.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/CTimeWheel.cpp.o: ../CTimeWheel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/admin/CLionProjects/client-new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/client.dir/CTimeWheel.cpp.o"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/CTimeWheel.cpp.o -c /home/admin/CLionProjects/client-new/CTimeWheel.cpp

CMakeFiles/client.dir/CTimeWheel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/CTimeWheel.cpp.i"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/admin/CLionProjects/client-new/CTimeWheel.cpp > CMakeFiles/client.dir/CTimeWheel.cpp.i

CMakeFiles/client.dir/CTimeWheel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/CTimeWheel.cpp.s"
	/opt/rh/devtoolset-9/root/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/admin/CLionProjects/client-new/CTimeWheel.cpp -o CMakeFiles/client.dir/CTimeWheel.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/main.cpp.o" \
"CMakeFiles/client.dir/CSocket.cpp.o" \
"CMakeFiles/client.dir/CCrypto.cpp.o" \
"CMakeFiles/client.dir/CCryptoAES.cpp.o" \
"CMakeFiles/client.dir/CProtocol.cpp.o" \
"CMakeFiles/client.dir/CProtocolHttp.cpp.o" \
"CMakeFiles/client.dir/CTimeWheel.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client: CMakeFiles/client.dir/main.cpp.o
client: CMakeFiles/client.dir/CSocket.cpp.o
client: CMakeFiles/client.dir/CCrypto.cpp.o
client: CMakeFiles/client.dir/CCryptoAES.cpp.o
client: CMakeFiles/client.dir/CProtocol.cpp.o
client: CMakeFiles/client.dir/CProtocolHttp.cpp.o
client: CMakeFiles/client.dir/CTimeWheel.cpp.o
client: CMakeFiles/client.dir/build.make
client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/admin/CLionProjects/client-new/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX executable client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client

.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd /home/admin/CLionProjects/client-new/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/admin/CLionProjects/client-new /home/admin/CLionProjects/client-new /home/admin/CLionProjects/client-new/cmake-build-debug /home/admin/CLionProjects/client-new/cmake-build-debug /home/admin/CLionProjects/client-new/cmake-build-debug/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend

