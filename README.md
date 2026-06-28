# SnazzCraft

--- All the dependencies are stated in dependancies.txt --- 


Building Project: \
    * Make sure to clone recursively to download all submodules \
    * Navigate to project directory \
    * Before compiliation I recommend editing CMakeLists and navigating to the add_compile_options lines, then adding an optimization flag (-Ofast is good). This especially helps on Linux based systems \
    * Note that you can remove the Release specifications if you are using GCC \
    * cmake -B build -DCMAKE_BUILD_TYPE=Release \
    * cmake --build build --config Release 

Running Project: \
    * For Linux run ./build/executable in the project's root directory \
    * For Windows run ./build/Release/executable.exe in the project's root directory

Controls: \
WASD              - Movement \
Space Bar         - Fly up \
Shift             - Fly down \
Q & E             - Rotate Yaw left and right respectively \
X & C             - Rotate Pitch Down (X) and up (C) \
Left Mouse Click  - Destroy voxel if able \
Right Mouse Click - Place voxel if able (number keys determine the voxel to be placed, so try out a few)
