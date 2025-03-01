
#ifndef PENGUIN_H
#define PENGUIN_H

#include <GL/glut.h>
#include <GL/glu.h>

#include <math.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include <cstdlib>

#include "Point.h"
#include "Bowl.h"

int SLICES = 8;
int STACKS = 8;

float frand() {
    return ((float) rand() / (float) RAND_MAX);
}

float bound(float val, float low, float high) {
    float range = high - low;
    while (val > high) {
        val -= range;
    }
    while (val < low) {
        val += range;
    }
    return val;
}

GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};

GLfloat deg2rad = 3.141592 / 180.0;
GLfloat rad2deg = 1.0 / deg2rad;

GLfloat maxFootAngle = 35.0;
GLfloat maxFlipperAngle = 35.0;
GLfloat maxHeadYawAngle = 35.0;

class Penguin;

typedef std::vector<Penguin> PenguinsContainer;
typedef PenguinsContainer::iterator penguinIt;
typedef PenguinsContainer::const_iterator penguinCi;

class Penguin: public Point {
    public:
        Penguin(GLfloat* pos, GLfloat* vel);
        Penguin(Point pos, Point vel);
        Penguin(const penguinCi fi);
        Penguin();
        ~Penguin() {
        }
        void drawNewt() const;
        void draw(bool isFocus, bool isPenguin) const;
        float getTheta() const {
            return rad2deg * atan2(velocity.getY(), velocity.getX());
        }
        float getPhi() const {
            return rad2deg * acos(velocity.getZ() / velocity.size());
        }
        Point getVel() const {
            return velocity;
        }
        Point getUp() const {
            float phi = getPhi();
            float theta = getTheta();
            return Point(
                sin(phi) * cos(theta),
                sin(phi) * sin(theta),
                cos(phi)
            );
        }
        float getSpeed() const;
        void timeStep(const PenguinsContainer& others, const Bowl& bowl);
        void otherPenguinTimeStep(const Penguin& other);

        void increaseDist() {
            closeDist *= 2.0;
            mediumDist *= 2.0;
            farDist *= 2.0;
        }
        ;
        void decreaseDist() {
            closeDist /= 2.0;
            mediumDist /= 2.0;
            farDist /= 2.0;
        }
        ;

        GLfloat getCloseDist() const {
            return closeDist;
        }
        GLfloat getMediumDist() const {
            return mediumDist;
        }
        GLfloat getFarDist() const {
            return farDist;
        }
        GLfloat getSize() const {
            return sizeOfPenguin;
        }
        void setColor(float x, float y, float z) {
            color.setX(x);
            color.setY(y);
            color.setZ(z);
        }
        Point getColor() const {
            return color;
        }
        float getHeadYaw() const {
            return headYawAngle;
        }
        void printDists();

        void setVel(Point newVel) {
            velocity = newVel;
        }
        void dump();
    private:
        Point velocity;
        Point color;
        GLfloat racismLevel;

        GLfloat acceleration;
        GLfloat maxSpeed;
        GLfloat accelerationFactor;

        GLfloat footAngle;
        GLfloat footAngleIncrement;
        GLfloat flipperAngle;
        GLfloat flipperAngleIncrement;
        GLfloat headYawAngle;
        GLfloat headYawAngleIncrement;

        GLfloat closeDist;
        GLfloat mediumDist;
        GLfloat farDist;

        GLUquadricObj *quad;
        GLuint texture;
        GLfloat sizeOfPenguin;

        void setup();
        float distanceTo(const Penguin& other) const;
        float velDiff(const penguinCi otherCi) const;

        void drawBody() const;
        void drawColorSetup() const;
        void drawHead() const;
        void drawFoot(bool right = true) const;
        void drawFeet() const;
        void drawFlipper(bool right = true) const;
        void drawFlippers() const;
        void moveAway(const Penguin& other);
        void moveTowards(const Penguin& other);

        void bounce(Point normalVector);
};

Penguin::Penguin() {
    setup();
}

Penguin::Penguin(GLfloat *pos, GLfloat* vel) {
    setup();
    setPos(pos);
    velocity.setPos(vel);
}

Penguin::Penguin(Point pos, Point vel) {
    setup();
    setPos(pos.getX(), pos.getY(), pos.getZ());
    velocity.setPos(vel.getX(), vel.getY(), vel.getZ());
}

void Penguin::setup() {
    racismLevel = 0.20;
    footAngle = 0.0;
    footAngleIncrement = 5.0;
    flipperAngle = 0.0;
    flipperAngleIncrement = 5.0;
    headYawAngle = 0.0;
    headYawAngleIncrement = 1.0;
    color = Point(0.1 * frand(), 0.1 * frand(), 0.1 * frand());

    float randVal = frand();
    if (randVal < 0.33) {
        color.setX(color.getX() * 10.0);
    } else if (randVal < 0.67) {
        color.setY(color.getY() * 10.0);
    } else {
        color.setZ(color.getZ() * 10.0);
    }

    sizeOfPenguin = 0.010;

    closeDist = 3.0 * sizeOfPenguin;
    mediumDist = 7.0 * sizeOfPenguin;
    farDist = 15.0 * sizeOfPenguin;

    acceleration = 0.020;
    maxSpeed = 0.020;
    accelerationFactor = 1.001;

    setPos(0.0, 0.0, 0.0);
    velocity.setPos(0.0, 0.0, 0.0);

    quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricTexture(quad, GL_TRUE);
}

void Penguin::drawColorSetup() const {
    GLfloat tmpColor[3] = {color.getX(), color.getY(), color.getZ()};
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
        glTranslatef(getX(), getY(), getZ());

        // Pitch and Yaw:
        glRotatef(getTheta(), 0.0, 0.0, 1.0);
        glRotatef(getPhi(), 0.0, 1.0, 0.0);

        glScalef(sizeOfPenguin, sizeOfPenguin, sizeOfPenguin);
        if (isFocus && !isPenguin) {
            drawNewt();
        } else {
            drawBody();
        }
    glPopMatrix();
}

void Penguin::drawNewt() const {
    glPushMatrix();
        glScalef(3.2, 1.4, 6.0);
        gluSphere(quad, 1.0, 8, 8);

        glPushMatrix();
            glScalef( 2.0, 0.1, 1.0 );
            glTranslatef( 0.25, 0.0, 0.0 );
            glutSolidSphere( 0.3, 8, 8 );
        glPopMatrix();

        glPushMatrix();
            glRotatef(225.0, 1, 0, 0);
            glTranslatef(0.0, 1.0, 0.0);
            glScalef(0.4, 1.0, 0.1);
            glutSolidSphere(1.0, 8, 8);
        glPopMatrix();

        glPushMatrix();
            glRotatef(315.0, 1, 0, 0);
            glTranslatef(0.0, 1.0, 0.0);
            glScalef(0.4, 1.0, 0.1);
            glutSolidSphere(1.0, 8, 8);
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

void Penguin::timeStep(const PenguinsContainer& others, const Bowl& bowl) {

    for (size_t ii = 0; ii < others.size(); ++ii) {
        // Skip self and others that you've already seen (do the bottom
        //   diagonal of the matrix, using 'break')
        //
        if (this == &others[ii]) {
            break;
        }

        // Modify current penguin based on another penguin:
        //
        otherPenguinTimeStep(others[ii]);
    }

    if (bowl.isOutside(*this)) {
        bounce(bowl.vecToEdge(*this));
    }

    // Move:
    //
    footAngleIncrement *= (footAngle > maxFootAngle || footAngle < -maxFootAngle) ? -1 : 1;
    flipperAngleIncrement *= (flipperAngle > maxFlipperAngle || flipperAngle < -maxFlipperAngle)  ? -1 : 1;
    headYawAngleIncrement *= (headYawAngle > maxHeadYawAngle || headYawAngle < -maxHeadYawAngle) ? -1 : 1;
    footAngle += footAngleIncrement; // flap foot
    flipperAngle += flipperAngleIncrement; // flap flipper
    headYawAngle += headYawAngleIncrement ; // turn head

    setX(getX() + velocity.getX());
    setY(getY() + velocity.getY());
    setZ(getZ() + velocity.getZ());

    velocity *= frand() < 0.5 ? accelerationFactor : 1.0 / accelerationFactor;
    if (getSpeed() > maxSpeed) {
        velocity.makeLength(maxSpeed);
    }
}

void Penguin::otherPenguinTimeStep(const Penguin& other) {
    // Skip differently-colored penguin:
    //
    Point colorDiff = getColor() - other.getColor();
    if (colorDiff.size() > racismLevel) {
        return;
    }

    float dist = distanceTo(other);
    if (dist < closeDist) {
        moveAway(other);
    } else if (dist > mediumDist && dist < farDist) {
        moveTowards(other);
    } else {
        ;// do nothing
    }
}

void Penguin::bounce(Point normalVector) {
    normalVector.makeLength(1.0);
    Point twoNTimesIDotN = normalVector * 2.0 * velocity.dot(normalVector);
    setVel(getVel() - twoNTimesIDotN);
}

float Penguin::distanceTo(const Penguin& other) const {
    Point diff = *this - other;
    return diff.getDist();
}

float Penguin::velDiff(const penguinCi otherCi) const {
    Point velDiff = velocity - otherCi->velocity;
    return velDiff.size();
}

void Penguin::moveAway(const Penguin& other) {
    GLfloat aa = acceleration;
    GLfloat bb = 1.0 - aa;
    velocity.setX(bb * velocity.getX() - aa * other.velocity.getX());
    velocity.setY(bb * velocity.getY() - aa * other.velocity.getY());
    velocity.setZ(bb * velocity.getZ() - aa * other.velocity.getZ());
}

void Penguin::moveTowards(const Penguin& other) {
    GLfloat aa = acceleration;
    GLfloat bb = 1.0 - aa;
    velocity.setX(bb * velocity.getX() + aa * other.velocity.getX());
    velocity.setY(bb * velocity.getY() + aa * other.velocity.getY());
    velocity.setZ(bb * velocity.getZ() + aa * other.velocity.getZ());
}

float Penguin::getSpeed() const {
    return velocity.size();
}

void Penguin::printDists() {
    cout << "Size: " << getSize() << ", Close radius: " << getCloseDist() << ", Far radius: " << getFarDist() << std::endl;
}

void Penguin::dump() {
    cout << getX() << " " << getY() << " " << getZ() << " ";
    cout << getVel().getX() << " " << getVel().getY() << " " << getVel().getZ() << endl;
}

#endif

