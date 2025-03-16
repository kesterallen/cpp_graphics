
#include "Point.h"

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
    cout << "x " << m_x << " y " << m_y << " z " << m_z << endl;
}

void Point::idump() const {
    cout << "x " << ix() << " y " << iy() << " z " << iz() << endl;
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

