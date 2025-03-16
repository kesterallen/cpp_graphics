#ifndef PENGUIN_H
#define PENGUIN_H

#include "Point.h"
#include "Bowl.h"
#include "VoxelGrid.h"
#include "Penguin.h"

#include <GL/glut.h>

float frand(); 

class VoxelGrid;
class Penguin;

typedef std::vector<Penguin> PenguinsContainer;
typedef PenguinsContainer::iterator penguinIt;
typedef PenguinsContainer::const_iterator penguinCi;

class Penguin {
    public:
        Penguin(GLfloat* pos, GLfloat* vel);
        Penguin(Point pos, Point vel);
        Penguin(const penguinCi fi);
        Penguin();
        ~Penguin() {
        }
        void drawNewt() const;
        void draw(bool isFocus, bool isPenguin) const;
        float getTheta() const;
        float getPhi() const ;

        const Point getPosRef() const {
            return m_position;
        }
        const Point getVelRef() const {
            return m_velocity;
        }
        void setVoxelAddress(Point address) {
            m_voxelAddress = address;
        }
        Point getVoxelAddress() const {
            return m_voxelAddress;
        }
        Point getVelCopy(float size) const {
            Point velocityCopy = Point(m_velocity);
            velocityCopy.makeLength(size);
            return velocityCopy;
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
        void timeStep(const Bowl& bowl, VoxelGrid& voxelGrid, bool ignoreOthers);
        void otherPenguinTimeStep(const Penguin& other);

        void increaseDist() {
            m_closeDist *= 2.0;
            m_mediumDist *= 2.0;
            m_farDist *= 2.0;
        }
        void decreaseDist() {
            m_closeDist /= 2.0;
            m_mediumDist /= 2.0;
            m_farDist /= 2.0;
        }
        GLfloat getCloseDist() const {
            return m_closeDist;
        }
        GLfloat getMediumDist() const {
            return m_mediumDist;
        }
        GLfloat getFarDist() const {
            return m_farDist;
        }
        GLfloat getScale() const {
            return penguinScale;
        }
        void setColor(float x, float y, float z) {
            color.setPos(x, y, z);
        }
        Point getColor() const {
            return color;
        }
        float getHeadYaw() const {
            return headYawAngle;
        }
        void printDists();

        void setVel(Point newVel) {
            m_velocity = newVel;
        }
        void dump();
    private:
        Point m_position;
        Point m_velocity;
        Point m_voxelAddress;
        Point color;
        GLfloat racism;

        GLfloat acceleration;
        GLfloat maxSpeed;
        GLfloat accelerationFactor;

        GLfloat footAngle;
        GLfloat footAngleIncrement;
        GLfloat flipperAngle;
        GLfloat flipperAngleIncrement;
        GLfloat headYawAngle;
        GLfloat headYawAngleIncrement;

        GLfloat m_closeDist;
        GLfloat m_mediumDist;
        GLfloat m_farDist;

        GLUquadricObj *quad;
        GLuint texture;
        GLfloat penguinScale;

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
        void accelerateAway(const Penguin& other, const bool away = true);
        void accelerateTowards(const Penguin& other);

        void bounce(Point normalVector);
};


#endif
