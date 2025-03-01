#ifndef BOWL_H
#define BOWL_H

#include <GL/glut.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <iostream>

#include "Point.h"

using std::cout;
using std::endl;

class Bowl {
    public:
        Bowl();
        ~Bowl() {
        }
        void draw() const;
        void timeStep();
        Point vecToEdge(const Point& pt) const;
        Point vecFromEdge(const Point& pt) const;
        Point positionOfEdge(const Point& pt) const;
        bool isOutside(const Point pt) const;
        bool isOutside(const double px, const double py, const double pz) const;

    private:
        Point center;
        double radius;
};

Bowl::Bowl() {
    center = Point(0.0, 0.0, 0.0);
    radius = 1.0;
}

void Bowl::draw() const {
    ; // do nothing
}

void Bowl::timeStep() {
    ; // do nothing
}

Point Bowl::vecToEdge(const Point& pt) const {
    Point out = pt;
    out.makeLength(radius);
    return out - pt;
}

Point Bowl::vecFromEdge(const Point& pt) const {
    Point vec = vecToEdge(pt);
    vec.reverse();
    return vec;
}

Point Bowl::positionOfEdge(const Point& pt) const {
    float distX = 0.5 - std::abs(pt.getX());
    float distY = 0.5 - std::abs(pt.getY());
    float distZ = 0.5 - std::abs(pt.getZ());

    Point edge = pt;

    if (distX < distY && distX < distZ) {
        edge.setX(distX);
    } else if (distY < distX && distY < distZ) {
        edge.setY(distY);
    } else {
        edge.setZ(distZ);
    }

    return edge;
}

bool Bowl::isOutside(const Point pt) const {
    return (pt - center).getDist() > radius;
}

#endif
