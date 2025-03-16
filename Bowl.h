#ifndef BOWL_H
#define BOWL_H

#include <stdlib.h>
#include <iostream>

#include "Point.h"

using std::cout;
using std::endl;

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

        Point center();
        double radius();

    private:
        Point m_center;
        double m_radius;
};

#endif
