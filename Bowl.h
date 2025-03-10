#ifndef BOWL_H
#define BOWL_H

#include <GL/glut.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <iostream>

#include <unordered_set>

#include "Point.h"

using std::cout;
using std::endl;

const int NUM_VOXEL_DIVS = 10;

class Penguin;

class Bowl {
    public:
        Bowl();
        ~Bowl() {}
        void draw() const;
        void timeStep();
        Point vecToEdge(const Point& pt) const;

        Point vecFromEdge(const Point& pt) const;
        Point positionOfEdge(const Point& pt) const;
        bool isOutside(const Point pt) const;
        bool isOutside(const double px, const double py, const double pz) const;

        Point getVoxelAddress(const Point& point) const;

    private:
        Point m_center;
        double m_radius;
        //std::unordered_set<Penguin*> m_voxels[NUM_VOXEL_DIVS][NUM_VOXEL_DIVS][NUM_VOXEL_DIVS];
        int m_voxels[NUM_VOXEL_DIVS][NUM_VOXEL_DIVS][NUM_VOXEL_DIVS];


};

Bowl::Bowl() {
    m_center = Point(0.0, 0.0, 0.0);
    m_radius = 1.0;
}

void Bowl::draw() const {
    ;
}

void Bowl::timeStep() {
    // TODO: move voxelgrid operations here
    ;
}

Point Bowl::vecToEdge(const Point& pt) const {
    Point out = pt;
    out.makeLength(m_radius);
    return out - pt;
}

Point Bowl::vecFromEdge(const Point& pt) const {
    Point vec = vecToEdge(pt);
    vec.reverse();
    return vec;
}

Point Bowl::positionOfEdge(const Point& pt) const {
    float dx = 0.5 - std::abs(pt.x());
    float dy = 0.5 - std::abs(pt.y());
    float dz = 0.5 - std::abs(pt.z());

    Point edge = pt;

    // TODO: figure out and document this:
    if (dx < dy && dx < dz) {
        edge.x(dx);
    } else if (dy < dx && dy < dz) {
        edge.y(dy);
    } else {
        edge.z(dz);
    }
    return edge;
}

bool Bowl::isOutside(const Point pt) const {
    return (pt - m_center).size() > m_radius;
}

Point Bowl::getVoxelAddress(const Point& point) const {
    Point address = (m_center - point + m_radius) * NUM_VOXEL_DIVS / m_radius / 2;
    return address;
    //m_voxels[vx.ix()][vx.iy()][vx.iz()] = 9;
}

class VoxelGrid {
    public:
        VoxelGrid();
        ~VoxelGrid() {}

        void updateContents(const Point& address);
    private:
        int m_voxels[NUM_VOXEL_DIVS][NUM_VOXEL_DIVS][NUM_VOXEL_DIVS];
};

VoxelGrid::VoxelGrid() {
    for (int i = 0; i < NUM_VOXEL_DIVS; ++i) {
        for (int j = 0; j < NUM_VOXEL_DIVS; ++j) {
            for (int k = 0; k < NUM_VOXEL_DIVS; ++k) {
                //m_voxels[i][j][k] = std::unordered_set<Penguin*>();
                m_voxels[i][j][k] = 0;
            }
        }
    }
}
void VoxelGrid::updateContents(const Point& address) {
    m_voxels[address.ix()][address.iy()][address.iz()] = 9;
}

#endif
