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
        ~Point() { }

        void draw() const;

        GLfloat x() const { return m_x; }
        GLfloat y() const { return m_y; }
        GLfloat z() const { return m_z; }
        int ix() const { return int(m_x); }
        int iy() const { return int(m_y); }
        int iz() const { return int(m_z); }

        void x(GLfloat x) { m_x = x; }
        void y(GLfloat y) { m_y = y; }
        void z(GLfloat z) { m_z = z; }

        // TODO:
        const Point getPos() const { return *this; }

        void setPos(GLfloat *pos);
        void setPos(Point pt);
        void setPos(GLfloat x, GLfloat y, GLfloat z);

        Point cross(const Point& other) const;
        float dot(const Point& other) const;

        void reverse();
        float size() const;
        void makeLength(float length);

        Point& operator+=(const Point& other);
        Point& operator-=(const Point& other);
        Point& operator*=(float factor);
        Point& operator*=(const Point& other);
        Point& operator/=(float factor);

        Point& operator=(const Point& other);
        Point operator+(const Point& other) const;
        Point operator+(const float factor) const;
        Point operator-(const Point& other) const;
        Point operator*(const float factor) const;
        Point operator/(const float factor) const;

        void dump() const;
    protected:
        GLfloat m_x;
        GLfloat m_y;
        GLfloat m_z;
};

Point::Point() {
    Point(0.0, 0.0, 0.0);
}

Point::Point(const GLfloat* position) {
    Point(position[0], position[1], position[2]);
}

Point::Point(GLfloat x, GLfloat y, GLfloat z) {
    m_x = x;
    m_y = y;
    m_z = z;
}

Point::Point(const Point& pt) {
    *this = pt;
}

void Point::draw() const {
    glPushMatrix();
        glTranslatef(m_x, m_y, m_z);
        glutWireCube(1.0);
    glPopMatrix();
}

void Point::setPos(Point point) {
    m_x = point.x();
    m_y = point.y();
    m_z = point.z();
}

void Point::setPos(GLfloat *position) {
    m_x = position[0];
    m_y = position[1];
    m_z = position[2];
}

void Point::setPos(GLfloat x, GLfloat y, GLfloat z) {
    m_x = x;
    m_y = y;
    m_z = z;
}

float Point::size() const {
    return sqrt(pow(m_x, 2) + pow(m_y, 2) + pow(m_z, 2));
}

void Point::dump() const {
    std::cout << "x " << m_x << " y " << m_y << " z " << m_z << std::endl;
}

void Point::reverse() {
    *this *= -1.0;
}

Point& Point::operator+=(const Point& other) {
    m_x += other.x();
    m_y += other.y();
    m_z += other.z();
    return *this;
}

Point& Point::operator-=(const Point& other) {
    m_x -= other.x();
    m_y -= other.y();
    m_z -= other.z();
    return *this;
}

Point& Point::operator*=(float factor) {
    m_x *= factor;
    m_y *= factor;
    m_z *= factor;
    return *this;
}

Point& Point::operator*=(const Point& other) {
    m_x *= other.x();
    m_y *= other.y();
    m_z *= other.z();
    return *this;
}

Point& Point::operator/=(float factor) {
    m_x /= factor;
    m_y /= factor;
    m_z /= factor;
    return *this;
}

Point& Point::operator=(const Point& other) {
    m_x = other.x();
    m_y = other.y();
    m_z = other.z();
    return *this;
}

Point Point::operator+(const Point& other) const {
    Point pt = Point(*this);
    pt += other;
    return pt;
}

Point Point::operator+(const float factor) const {
    Point pt = Point(*this);
    pt.x(pt.x() + factor);
    pt.y(pt.y() + factor);
    pt.z(pt.z() + factor);
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

void Point::makeLength(float length) {
    float factor = length / size();
    setPos(m_x * factor, m_y * factor, m_z * factor);
}

Point Point::cross(const Point& other) const {
    Point out = Point();
    out.setPos(
        m_y * other.z() - m_z * other.y(),
        m_z * other.x() - m_x * other.z(),
        m_x * other.y() - m_y * other.x()
    );
    return out;
}

float Point::dot(const Point& other) const {
    float out = m_x * other.x() + m_y * other.y() + m_z * other.z();
    return out;
}

#endif
