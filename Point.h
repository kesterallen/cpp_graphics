#ifndef POINT_H
#define POINT_H

#include <GL/glut.h>
#include <GL/glu.h>

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Point {
    public:
        Point();
        Point(const Point& pt);
        Point(const GLfloat* pos);
        Point(GLfloat x, GLfloat y, GLfloat z);
        ~Point() {
        }
        void draw() const;
        GLfloat getX() const {
            return m_pos[0];
        }
        GLfloat getY() const {
            return m_pos[1];
        }
        GLfloat getZ() const {
            return m_pos[2];
        }
        float getDist() const;

        Point getPos() const {
            return Point(m_pos);
        }
        void setPos(GLfloat *pos);
        void setPos(Point pt);
        void setPos(GLfloat px, GLfloat py, GLfloat pz);
        void moveBy(Point pt);

        Point cross(const Point& other) const;
        float dot(const Point& other) const;

        void makeLength(float wantedLength);

        void reverse();
        float size() const;

        void dump() const;

        Point& operator+=(const Point& other);
        Point& operator-=(const Point& other);
        Point& operator*=(float factor);
        Point& operator*=(const Point& other);
        Point& operator/=(float factor);

        Point& operator=(const Point& other);
        Point operator+(const Point& other) const;
        Point operator-(const Point& other) const;
        Point operator*(const float factor) const;
        Point operator/(const float factor) const;

        void setX(GLfloat pos) {
            m_pos[0] = pos;
        }
        void setY(GLfloat pos) {
            m_pos[1] = pos;
        }
        void setZ(GLfloat pos) {
            m_pos[2] = pos;
        }
    protected:
        GLfloat m_pos[3];
};

Point::Point() {
    Point(0.0, 0.0, 0.0);
}

Point::Point(const GLfloat* pos) {
    Point(pos[0], pos[1], pos[2]);
}

Point::Point(GLfloat x, GLfloat y, GLfloat z) {
    setX(x);
    setY(y);
    setZ(z);
}

Point::Point(const Point& pt) {
    *this = pt;
}

void Point::draw() const {
    glPushMatrix();
    glTranslatef(getX(), getY(), getZ());
    glutWireCube(1.0);
    glPopMatrix();
}

void Point::setPos(Point pt) {
    setX(pt.getX());
    setY(pt.getY());
    setZ(pt.getZ());
}

void Point::setPos(GLfloat *pos) {
    setX(pos[0]);
    setY(pos[1]);
    setZ(pos[2]);
}

void Point::setPos(GLfloat px, GLfloat py, GLfloat pz) {
    setX(px);
    setY(py);
    setZ(pz);
}

float Point::getDist() const {
    return sqrt(pow(getX(), 2) + pow(getY(), 2) + pow(getZ(), 2));
}

void Point::dump() const {
    std::cout << "x " << getX() << " y " << getY() << " z " << getZ() << std::endl;
}

void Point::reverse() {
    *this *= -1.0;
}

float Point::size() const {
    return getDist();
}

Point& Point::operator+=(const Point& other) {
    setX(getX() + other.getX());
    setY(getY() + other.getY());
    setZ(getZ() + other.getZ());
    return *this;
}

Point& Point::operator-=(const Point& other) {
    setX(getX() - other.getX());
    setY(getY() - other.getY());
    setZ(getZ() - other.getZ());
    return *this;
}

Point& Point::operator *=(float factor) {
    setX(getX() * factor);
    setY(getY() * factor);
    setZ(getZ() * factor);
    return *this;
}

Point& Point::operator *=(const Point& other) {
    setX(getX() * other.getX());
    setY(getY() * other.getY());
    setZ(getZ() * other.getZ());
    return *this;
}

Point& Point::operator /=(float factor) {
    setX(getX() / factor);
    setY(getY() / factor);
    setZ(getZ() / factor);
    return *this;
}

Point& Point::operator=(const Point& other) {
    setX(other.getX());
    setY(other.getY());
    setZ(other.getZ());
    return *this;
}

Point Point::operator+(const Point& other) const {
    Point pt = Point(*this);
    pt += other;
    return pt;
}

Point Point::operator-(const Point& other) const {
    Point pt = Point(*this);
    pt -= other;
    return pt;
}

Point Point::operator/(const float factor) const {
    Point pt = Point(*this);
    pt /= factor;
    return pt;
}

Point Point::operator*(const float factor) const {
    Point pt = Point(*this);
    pt *= factor;
    return pt;
}

void Point::makeLength(float wantedLength) {
    float dist = getDist();
    setPos(getX() * wantedLength / dist, getY() * wantedLength / dist, getZ() * wantedLength / dist);
}

Point Point::cross(const Point& other) const {
    Point out = Point();
    out.setPos(getY() * other.getZ() - getZ() * other.getY(), getZ() * other.getX() - getX() * other.getZ(), getX() * other.getY() - getY() * other.getX());
    return out;
}

float Point::dot(const Point& other) const {
    float out = (getX() * other.getX()) + (getY() * other.getY()) + (getZ() * other.getZ());
    return out;
}

#endif
