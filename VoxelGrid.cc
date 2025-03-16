
#include <map>

#include "VoxelGrid.h"
#include "Point.h"

using std::cout;
using std::endl;

VoxelGrid::VoxelGrid() {
    for (int i = 0; i < NUM_VOXEL_DIVS; ++i) {
        for (int j = 0; j < NUM_VOXEL_DIVS; ++j) {
            for (int k = 0; k < NUM_VOXEL_DIVS; ++k) {
                m_voxels[i][j][k] = PenguinsContainer();
            }
        }
    }
    clear();
}

void VoxelGrid::clear() {
    for (int i = 0; i < NUM_VOXEL_DIVS; ++i) {
        for (int j = 0; j < NUM_VOXEL_DIVS; ++j) {
            for (int k = 0; k < NUM_VOXEL_DIVS; ++k) {
                m_voxels[i][j][k].clear();
            }
        }
    }
}

void VoxelGrid::updateContents(Point address, Penguin penguin) {
    m_voxels[address.ix()][address.iy()][address.iz()].push_back(penguin);
}

PenguinsContainer VoxelGrid::getContents(Point address) const {
    PenguinsContainer others = m_voxels[address.ix()][address.iy()][address.iz()];
    return others;
}

Point VoxelGrid::getVoxelAddress(const Point& point, const Point& lowCorner, const Point& highCorner) const {
    Point address = (point - lowCorner) / (highCorner - lowCorner).size() * NUM_VOXEL_DIVS;
    //Point address = (center - point + radius) * NUM_VOXEL_DIVS / radius / 2;
    return address;
}

void VoxelGrid::printStats() const {
    map<int, int> histogram;
    for (int i = 0; i < NUM_VOXEL_DIVS; ++i) {
        for (int j = 0; j < NUM_VOXEL_DIVS; ++j) {
            for (int k = 0; k < NUM_VOXEL_DIVS; ++k) {

                int count = m_voxels[i][j][k].size();
                if (histogram.find(count) == histogram.end()) {
                    histogram[count] = 0;
                }
                histogram[count]++;

            }
        }
    }

    for (auto it = histogram.begin(); it != histogram.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    cout << endl;
}

