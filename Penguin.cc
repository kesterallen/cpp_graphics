
#include <GL/glut.h>
#include <GL/glu.h>


#include <math.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include <cstdlib>

#include "VoxelGrid.h"
#include "Point.h"
#include "Bowl.h"
#include "Penguin.h"

int SLICES = 8;
int STACKS = 8;

GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};

GLfloat maxFootAngle = 35.0;
GLfloat maxFlipperAngle = 35.0;
GLfloat maxHeadYawAngle = 35.0;

GLfloat deg2rad = 3.141592 / 180.0;
GLfloat rad2deg = 1.0 / deg2rad;

float frand() { return ((float) rand() / (float) RAND_MAX); }

Penguin::Penguin() {
    setup();
}

Penguin::Penguin(GLfloat *pos, GLfloat* vel) {
    setup();
    m_position.setPos(pos);
    m_velocity.setPos(vel);
}

Penguin::Penguin(Point pos, Point vel) {
    setup();
    m_position.setPos(pos);
    m_velocity.setPos(vel);
}

void Penguin::setup() {
    m_id = 0;
    racism = 0.20;
    footAngle = 0.0;
    footAngleIncrement = 5.0;
    flipperAngle = 0.0;
    flipperAngleIncrement = 5.0;
    headYawAngle = 0.0;
    headYawAngleIncrement = 1.0;
    color = Point(0.1 * frand(), 0.1 * frand(), 0.1 * frand());

    float randVal = frand();
    if (randVal < 0.33) {
        color.x(color.x() * 10.0);
    } else if (randVal < 0.67) {
        color.y(color.y() * 10.0);
    } else {
        color.z(color.z() * 10.0);
    }

    penguinScale = 0.010;

    m_closeDist = 3.0 * penguinScale;
    m_mediumDist = 7.0 * penguinScale;
    m_farDist = 15.0 * penguinScale;

    acceleration = 0.020;
    maxSpeed = 0.020;
    accelerationFactor = 1.001;

    m_position.setPos(0.0, 0.0, 0.0);
    m_velocity.setPos(0.0, 0.0, 0.0);
    m_voxelAddress.setPos(0.0, 0.0, 0.0);

    quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricTexture(quad, GL_TRUE);
}

float Penguin::getTheta() const {
    return rad2deg * atan2(m_velocity.y(), m_velocity.x());
}

float Penguin::getPhi() const {
    return rad2deg * acos(m_velocity.z() / m_velocity.size());
}

void Penguin::drawColorSetup() const {
    GLfloat tmpColor[3] = {color.x(), color.y(), color.z()};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  tmpColor);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   tmpColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  tmpColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, tmpColor);
    //glColor3fv(m_color);
}

void Penguin::draw(bool isFocus, bool isPenguin) const {
    glPushMatrix();
        glEnable(GL_DEPTH_TEST);
        drawColorSetup();

        // Position
        glTranslatef(m_position.x(), m_position.y(), m_position.z());

        // Pitch and Yaw:
        glRotatef(getTheta(), 0.0, 0.0, 1.0);
        glRotatef(getPhi(), 0.0, 1.0, 0.0);

        glScalef(penguinScale, penguinScale, penguinScale);
        if (isPenguin) {
            drawBody();
        } else {
            drawNewt();
        }
    glPopMatrix();
}

void Penguin::drawNewt() const {
    glPushMatrix();
        // Body
        glPushMatrix();
            glScalef(1.0, 2.0, 6.0);
            gluSphere(quad, 1.0, SLICES, STACKS);
        glPopMatrix();

        // head
        glPushMatrix();
            // head
            glTranslatef(0.0, 0.0, 6.0);
            glutSolidSphere(1.0, SLICES, STACKS);

            // Head antenae
            for (int idir = 0; idir < 2; ++idir) {
                float offset = idir == 0 ? 1.2 : -1.2;
                for (int irot = 0; irot < 3; ++irot) {
                    float angle = (irot - 1) * 30.0;
                    glPushMatrix();
                        glRotatef(angle, 0.0, 0.0, 1.0);
                        glTranslatef(0.0, offset, 0.0);
                        glScalef(1.0, 5.0, 1.0);
                        glutSolidSphere(0.3, SLICES, STACKS);
                    glPopMatrix();
                }
            }

            // eyes:
            for (int i = 0; i < 2; ++i) {
                float offset = i == 0 ? 0.7 : -0.7;
                glPushMatrix();
                    glTranslatef(0.0, offset, 1.0);
                    glutSolidSphere(0.2, SLICES, STACKS);
                glPopMatrix();
            }
        glPopMatrix();

        // legs
        glPushMatrix();
            for (int ileg = 0; ileg < 4; ++ileg) {
                float right = ileg < 2 ? 4.0 : -4.0;
                float front = ileg % 2 == 0 ? 3.0 : 0.0;
                glPushMatrix();
                    glTranslatef(0.0, right, front);
                    glScalef(1.0, 5.0, 1.0);
                    glutSolidSphere(1.0, SLICES, STACKS);
                glPopMatrix();
            }
        glPopMatrix();

    glPopMatrix();
}

void Penguin::drawBody() const {
    // Body:
    glPushMatrix();
        glRotatef(-90.0, 0.0, 1.0, 0.0);

        float scale = 2.0;
        glScalef(scale, scale, scale);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Tush:
        gluSphere(quad, 1.0, SLICES, STACKS);

        drawFeet();

        // Cone-ish body (base, top, height)
        gluCylinder(quad, 1.0, 0.2, 2.0, SLICES, STACKS);

        drawFlippers();
        drawHead();

    glPopMatrix();
}

void Penguin::drawHead() const {
    glPushMatrix();
        glTranslatef(0.0, 0.0, 2.0);
        gluSphere(quad, 0.4, SLICES, STACKS);

        glRotatef(headYawAngle, 0.0, 0.0, 1.0);

        // Eyes:
        float eyewidth = 0.3;
        glPushMatrix();
            glTranslatef(0.2, eyewidth, 0.0);
            gluSphere(quad, 0.1, SLICES, STACKS);
            glTranslatef(0.0, -2.0 * eyewidth, 0.0);
            gluSphere(quad, 0.1, SLICES, STACKS);
        glPopMatrix();

        //  Beak
        glRotatef(90.0, 0.0, 1.0, 0.0);
        glutSolidCone(0.25, 1.0, SLICES, STACKS);
    glPopMatrix();
}

void Penguin::drawFoot(bool right) const {
    float length = 0.5;
    float thickness = 2.0;
    float width = 4.0;

    int dir = right ? 1 : -1;

    glPushMatrix();
        glTranslatef(0.8, dir * 0.6, -0.5);
        glRotatef(dir * 30.0, 0.0, 0.0, 1.0);
        glRotatef(20.0, 0.0, 1.0, 0.0);
        glRotatef(dir * footAngle, 0.0, 1.0, 0.0);
        glScalef(width, thickness, length);
        gluSphere(quad, 0.2, SLICES, STACKS);
    glPopMatrix();
}

void Penguin::drawFeet() const {
    glPushMatrix();
        drawFoot(true);
        drawFoot(false);
    glPopMatrix();
}

void Penguin::drawFlipper(bool right) const {
    float length = 5.0;
    float thickness = 1.0;
    float width = 2.0;

    int dir = right ? 1 : -1;

    glPushMatrix();
        glTranslatef(0.0, dir * 0.55, 0.0);
        glRotatef(dir * (45.0 + flipperAngle), 1.0, 0.0, 0.0);
        glTranslatef(0.0, 0.0, -1.0);
        glScalef(width, thickness, length);
        gluSphere(quad, 0.2, SLICES, STACKS);
    glPopMatrix();
}

void Penguin::drawFlippers() const {
    glPushMatrix();
        glTranslatef(0.0, 0.0, 1.0);
        drawFlipper(true);
        drawFlipper(false);
    glPopMatrix();
}

void Penguin::timeStep(const Bowl& bowl) {
    if (bowl.isOutside(m_position)) {
        bounce(bowl.vecToEdge(m_position));
    }

    // Move body parts:
    //
    footAngleIncrement *= (footAngle > maxFootAngle || footAngle < -maxFootAngle) ? -1 : 1;
    flipperAngleIncrement *= (flipperAngle > maxFlipperAngle || flipperAngle < -maxFlipperAngle)  ? -1 : 1;
    headYawAngleIncrement *= (headYawAngle > maxHeadYawAngle || headYawAngle < -maxHeadYawAngle) ? -1 : 1;
    footAngle += footAngleIncrement; // flap foot
    flipperAngle += flipperAngleIncrement; // flap flipper
    headYawAngle += headYawAngleIncrement ; // turn head

    // move / accelerate:
    //
    m_position += m_velocity;
    m_velocity *= (frand() < 0.5) ? accelerationFactor : 1.0 / accelerationFactor;

    if (getSpeed() > maxSpeed) {
        m_velocity.makeLength(maxSpeed);
    }
}

void Penguin::otherPenguinTimeStep(const Penguin& other) {
    if (this->id() == other.id()) {
        return;
    }

    // Skip differently-colored penguin:
    //
    Point colorDiff = getColor() - other.getColor();
    if (colorDiff.size() > racism) {
        return;
    }

    float dist = distanceTo(other);
    if (dist < m_closeDist) {
        accelerateAway(other);
    } else if (dist > m_mediumDist && dist < m_farDist) {
        accelerateTowards(other);
    } else {
        ;// do nothing
    }
}

void Penguin::bounce(Point normalVector) {
    normalVector.makeLength(1.0);
    Point twoNTimesIDotN = normalVector * 2.0 * m_velocity.dot(normalVector);
    setVel(m_velocity - twoNTimesIDotN);
}

float Penguin::distanceTo(const Penguin& other) const {
    Point diff = m_position - other.m_position;
    return diff.size();
}

float Penguin::velDiff(const pitc itOther) const {
    Point velDiff = m_velocity - itOther->m_velocity;
    return velDiff.size();
}

void Penguin::accelerateAway(const Penguin& other, bool away) {
    // 8pm: 0.00967 ->  (11 hrs later): 0.00113 
    // 
    Point v1 = m_velocity * (1.0 - acceleration);
    Point v2 = other.m_velocity * acceleration;
    float direction = away ? -1.0 : 1.0;
    Point orig = m_velocity;
    m_velocity = v1 + v2 * direction;
    m_velocity.makeLength(orig.size());
}

void Penguin::accelerateTowards(const Penguin& other) {
    accelerateAway(other, false);
}

float Penguin::getSpeed() const {
    return m_velocity.size();
}

void Penguin::printDists() {
    cout << "Size: " << getScale() 
         << ", Close radius: " << getCloseDist() 
         << ", Far radius: " << getFarDist() << std::endl;
}

void Penguin::dump() {
    cout << m_position.x() << " " << m_position.y() << " " << m_position.z() << " ";
    cout << m_velocity.x() << " " << m_velocity.y() << " " << m_velocity.z() << endl;
}
