#ifndef VOXELGRID_H
#define VOXELGRID_H

#include "Penguin.h"

const int NUM_VOXEL_DIVS = 10;

class Point;
class Penguin;

class VoxelGrid {
    public:
        VoxelGrid();
        ~VoxelGrid() {}

        Point getVoxelAddress(const Point& point, const Point& lowCorner, const Point& highCorner) const;
        void updateContents(Point address, Penguin penguin);
        PenguinsContainer getContents(Point address) const;
        void printStats() const;
        void clear();
    private:
         PenguinsContainer m_voxels[NUM_VOXEL_DIVS][NUM_VOXEL_DIVS][NUM_VOXEL_DIVS];
};

#endif
