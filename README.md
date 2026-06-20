# SnazzCraft

--- All the dependencies are stated in dependancies.txt --- 


Building Project: \
    [This may be different on windows] \
    * Navigate to project directory \
    * cmake -B build -DCMAKE_BUILD_TYPE=Release \
    * cmake --build build -j$(nproc) \
    * ./build/bin/executable 


Controls:

WASD              - Movement
Space Bar         - Fly up
Shift             - Fly down
Q & E             - Rotate Yaw left and right respectively
X & C             - Rotate Pitch Down (X) and up (C)
Left Mouse Click  - Destroy voxel if able
Right Mouse Click - Place voxel if able (number keys determine the voxel to be placed, so try out a few)
