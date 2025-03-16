
#include "Bowl.h"

Bowl::Bowl() {
    m_center = Point(0.0, 0.0, 0.0);
    m_radius = 1.0;
}

void Bowl::draw() const {
    ;
}

void Bowl::timeStep() {
    // TODO: move voxelgrid operations here??
    ;
}

Point Bowl::center() { return m_center; }
double Bowl::radius() { return m_radius; }

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

