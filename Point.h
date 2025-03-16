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
        void idump() const;
    protected:
        GLfloat m_x;
        GLfloat m_y;
        GLfloat m_z;
};
#endif
