#include "voxel.hpp"

SnazzCraft::Voxel::Voxel(unsigned int X, unsigned int Y, unsigned int Z, unsigned int ID)
{
    this->Position[0] = X;
    this->Position[1] = Y;
    this->Position[2] = Z;
    this->ID = ID;
}

