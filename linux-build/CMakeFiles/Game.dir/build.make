# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /home/john/cmake_proj/game

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/john/cmake_proj/game/linux-build

# Include any dependencies generated for this target.
include CMakeFiles/Game.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Game.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Game.dir/flags.make

CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o: ../Classes/AppDelegate.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/john/cmake_proj/game/linux-build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o -c /home/john/cmake_proj/game/Classes/AppDelegate.cpp

CMakeFiles/Game.dir/Classes/AppDelegate.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/Classes/AppDelegate.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/john/cmake_proj/game/Classes/AppDelegate.cpp > CMakeFiles/Game.dir/Classes/AppDelegate.cpp.i

CMakeFiles/Game.dir/Classes/AppDelegate.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/Classes/AppDelegate.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/john/cmake_proj/game/Classes/AppDelegate.cpp -o CMakeFiles/Game.dir/Classes/AppDelegate.cpp.s

CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o.requires:
.PHONY : CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o.requires

CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o.provides: CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o.requires
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o.provides.build
.PHONY : CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o.provides

CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o.provides.build: CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o

CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o: ../Classes/SceneLayer.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/john/cmake_proj/game/linux-build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o -c /home/john/cmake_proj/game/Classes/SceneLayer.cpp

CMakeFiles/Game.dir/Classes/SceneLayer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/Classes/SceneLayer.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/john/cmake_proj/game/Classes/SceneLayer.cpp > CMakeFiles/Game.dir/Classes/SceneLayer.cpp.i

CMakeFiles/Game.dir/Classes/SceneLayer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/Classes/SceneLayer.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/john/cmake_proj/game/Classes/SceneLayer.cpp -o CMakeFiles/Game.dir/Classes/SceneLayer.cpp.s

CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o.requires:
.PHONY : CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o.requires

CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o.provides: CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o.requires
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o.provides.build
.PHONY : CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o.provides

CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o.provides.build: CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o

CMakeFiles/Game.dir/Classes/GameScene.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/Classes/GameScene.cpp.o: ../Classes/GameScene.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/john/cmake_proj/game/linux-build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Game.dir/Classes/GameScene.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/Classes/GameScene.cpp.o -c /home/john/cmake_proj/game/Classes/GameScene.cpp

CMakeFiles/Game.dir/Classes/GameScene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/Classes/GameScene.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/john/cmake_proj/game/Classes/GameScene.cpp > CMakeFiles/Game.dir/Classes/GameScene.cpp.i

CMakeFiles/Game.dir/Classes/GameScene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/Classes/GameScene.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/john/cmake_proj/game/Classes/GameScene.cpp -o CMakeFiles/Game.dir/Classes/GameScene.cpp.s

CMakeFiles/Game.dir/Classes/GameScene.cpp.o.requires:
.PHONY : CMakeFiles/Game.dir/Classes/GameScene.cpp.o.requires

CMakeFiles/Game.dir/Classes/GameScene.cpp.o.provides: CMakeFiles/Game.dir/Classes/GameScene.cpp.o.requires
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/Classes/GameScene.cpp.o.provides.build
.PHONY : CMakeFiles/Game.dir/Classes/GameScene.cpp.o.provides

CMakeFiles/Game.dir/Classes/GameScene.cpp.o.provides.build: CMakeFiles/Game.dir/Classes/GameScene.cpp.o

CMakeFiles/Game.dir/Classes/Soldier.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/Classes/Soldier.cpp.o: ../Classes/Soldier.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/john/cmake_proj/game/linux-build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Game.dir/Classes/Soldier.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/Classes/Soldier.cpp.o -c /home/john/cmake_proj/game/Classes/Soldier.cpp

CMakeFiles/Game.dir/Classes/Soldier.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/Classes/Soldier.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/john/cmake_proj/game/Classes/Soldier.cpp > CMakeFiles/Game.dir/Classes/Soldier.cpp.i

CMakeFiles/Game.dir/Classes/Soldier.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/Classes/Soldier.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/john/cmake_proj/game/Classes/Soldier.cpp -o CMakeFiles/Game.dir/Classes/Soldier.cpp.s

CMakeFiles/Game.dir/Classes/Soldier.cpp.o.requires:
.PHONY : CMakeFiles/Game.dir/Classes/Soldier.cpp.o.requires

CMakeFiles/Game.dir/Classes/Soldier.cpp.o.provides: CMakeFiles/Game.dir/Classes/Soldier.cpp.o.requires
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/Classes/Soldier.cpp.o.provides.build
.PHONY : CMakeFiles/Game.dir/Classes/Soldier.cpp.o.provides

CMakeFiles/Game.dir/Classes/Soldier.cpp.o.provides.build: CMakeFiles/Game.dir/Classes/Soldier.cpp.o

CMakeFiles/Game.dir/proj.linux/main.cpp.o: CMakeFiles/Game.dir/flags.make
CMakeFiles/Game.dir/proj.linux/main.cpp.o: ../proj.linux/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/john/cmake_proj/game/linux-build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Game.dir/proj.linux/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Game.dir/proj.linux/main.cpp.o -c /home/john/cmake_proj/game/proj.linux/main.cpp

CMakeFiles/Game.dir/proj.linux/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Game.dir/proj.linux/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/john/cmake_proj/game/proj.linux/main.cpp > CMakeFiles/Game.dir/proj.linux/main.cpp.i

CMakeFiles/Game.dir/proj.linux/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Game.dir/proj.linux/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/john/cmake_proj/game/proj.linux/main.cpp -o CMakeFiles/Game.dir/proj.linux/main.cpp.s

CMakeFiles/Game.dir/proj.linux/main.cpp.o.requires:
.PHONY : CMakeFiles/Game.dir/proj.linux/main.cpp.o.requires

CMakeFiles/Game.dir/proj.linux/main.cpp.o.provides: CMakeFiles/Game.dir/proj.linux/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Game.dir/build.make CMakeFiles/Game.dir/proj.linux/main.cpp.o.provides.build
.PHONY : CMakeFiles/Game.dir/proj.linux/main.cpp.o.provides

CMakeFiles/Game.dir/proj.linux/main.cpp.o.provides.build: CMakeFiles/Game.dir/proj.linux/main.cpp.o

# Object files for target Game
Game_OBJECTS = \
"CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o" \
"CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o" \
"CMakeFiles/Game.dir/Classes/GameScene.cpp.o" \
"CMakeFiles/Game.dir/Classes/Soldier.cpp.o" \
"CMakeFiles/Game.dir/proj.linux/main.cpp.o"

# External object files for target Game
Game_EXTERNAL_OBJECTS =

bin/Game: CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o
bin/Game: CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o
bin/Game: CMakeFiles/Game.dir/Classes/GameScene.cpp.o
bin/Game: CMakeFiles/Game.dir/Classes/Soldier.cpp.o
bin/Game: CMakeFiles/Game.dir/proj.linux/main.cpp.o
bin/Game: CMakeFiles/Game.dir/build.make
bin/Game: lib/libcocos2d.a
bin/Game: /usr/lib/x86_64-linux-gnu/libGLU.so
bin/Game: /usr/lib/x86_64-linux-gnu/libGL.so
bin/Game: /usr/lib/x86_64-linux-gnu/libGLEW.so
bin/Game: ../cocos2d/external/linux-specific/fmod/prebuilt/64-bit/libfmodex64.so
bin/Game: ../cocos2d/external/linux-specific/fmod/prebuilt/64-bit/libfmodexL64.so
bin/Game: lib/libunzip.a
bin/Game: /usr/lib/x86_64-linux-gnu/libz.so
bin/Game: ../cocos2d/external/jpeg/prebuilt/linux/64-bit/libjpeg.a
bin/Game: /usr/lib/x86_64-linux-gnu/libpng.so
bin/Game: /usr/lib/x86_64-linux-gnu/libz.so
bin/Game: ../cocos2d/external/jpeg/prebuilt/linux/64-bit/libjpeg.a
bin/Game: /usr/lib/x86_64-linux-gnu/libpng.so
bin/Game: ../cocos2d/external/tiff/prebuilt/linux/64-bit/libtiff.a
bin/Game: lib/libtinyxml2.a
bin/Game: ../cocos2d/external/freetype2/prebuilt/linux/64-bit/libfreetype.a
bin/Game: ../cocos2d/external/websockets/prebuilt/linux/64-bit/libwebsockets.a
bin/Game: lib/libxxhash.a
bin/Game: ../cocos2d/external/webp/prebuilt/linux/64-bit/libwebp.a
bin/Game: ../cocos2d/external/chipmunk/prebuilt/linux/64-bit/libchipmunk.a
bin/Game: lib/libbullet.a
bin/Game: lib/librecast.a
bin/Game: CMakeFiles/Game.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bin/Game"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Game.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Game.dir/build: bin/Game
.PHONY : CMakeFiles/Game.dir/build

CMakeFiles/Game.dir/requires: CMakeFiles/Game.dir/Classes/AppDelegate.cpp.o.requires
CMakeFiles/Game.dir/requires: CMakeFiles/Game.dir/Classes/SceneLayer.cpp.o.requires
CMakeFiles/Game.dir/requires: CMakeFiles/Game.dir/Classes/GameScene.cpp.o.requires
CMakeFiles/Game.dir/requires: CMakeFiles/Game.dir/Classes/Soldier.cpp.o.requires
CMakeFiles/Game.dir/requires: CMakeFiles/Game.dir/proj.linux/main.cpp.o.requires
.PHONY : CMakeFiles/Game.dir/requires

CMakeFiles/Game.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Game.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Game.dir/clean

CMakeFiles/Game.dir/depend:
	cd /home/john/cmake_proj/game/linux-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/john/cmake_proj/game /home/john/cmake_proj/game /home/john/cmake_proj/game/linux-build /home/john/cmake_proj/game/linux-build /home/john/cmake_proj/game/linux-build/CMakeFiles/Game.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Game.dir/depend

