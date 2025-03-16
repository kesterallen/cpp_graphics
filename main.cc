
#include "VoxelGrid.h"
#include "Penguin.h"
#include "Bowl.h"

int timeStepSize = 25; // msec
int winWidth = 800; // pix
int winHeight = 800; // pix
GLfloat viewAngle = 90.0; // deg

bool ignoreOthers = false;
bool isPaused = false;
bool isPenguin = true;
int iView = 0;

GLfloat posLight[4] = {1.0, 0.0, 0.0, 0.0};
GLfloat ambientLight[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat diffuseLight[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat specularLight[4] = {1.0, 1.0, 1.0, 1.0};

int nPenguins = 100;
int nViewpointPenguins = 5;

GLfloat cameraDist = 1.0;
GLfloat cameraTheta = 0.0;
GLfloat cameraThetaStepDelta = 0.002;
GLfloat cameraThetaStep = cameraThetaStepDelta;
GLfloat cameraPhi = 45.0;
GLfloat cameraPhiStep = 0.1;
Point eye, look, up;

Bowl bowl;
VoxelGrid voxelGrid;
PenguinsContainer penguins;

void initPenguins() {
    penguins.clear();
    for (int ii = 0; ii < nPenguins; ++ii) {
        GLfloat pos[3] = {frand() - .5, frand() - .5, frand() - .5};
        GLfloat vel[3] = {0.02 * (frand() - .5), 0.02 * (frand() - .5), 0.02 * (frand() - .5)};
        penguins.push_back(Penguin(pos, vel));
    }
    penguins[0].printDists();
}

void initGl() {
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);

    glLightfv(GL_LIGHT0, GL_POSITION, posLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);
}

void display() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLightfv(GL_LIGHT0, GL_POSITION, posLight);
    glLoadIdentity();

    // Set the viewpoint to be either the outside of bowl,
    // or from a penguin's POV
    if (iView > 0) {
        Penguin penguin = penguins[iView];

        const Point p = penguin.getPosRef();
        Point v = penguin.getVelCopy(penguin.getScale() * 20.0);

        eye = p - v;
        look = p + v; // TODO rotate *this* by p.getHeadYaw()?
        up = Point(0.0, 0.0, 1.0);
        glRotatef(penguin.getHeadYaw(), 0.0, -1.0, 0.0);
    }
    gluLookAt(
        eye.x(), eye.y(), eye.z(),
        look.x(), look.y(), look.z(),
        up.x(), up.y(), up.z()
    );

    // Draw bowl and penguins:
    //
    bowl.draw();
    int i = 0;
    for (penguinIt it = penguins.begin(); it != penguins.end(); ++it) {
        it->draw(i++ == iView, isPenguin);
    }

    glutSwapBuffers();
}

void animateNothing(int id) {
    glutPostRedisplay();
    glutTimerFunc(timeStepSize, animateNothing, id);
}

void animation(int id) {

    if (!isPaused) {

        // update voxels
        voxelGrid.clear();
        Point lowCorner = Point(-bowl.radius(), -bowl.radius(), -bowl.radius());
        Point highCorner = Point(bowl.radius(), bowl.radius(), bowl.radius());

        for (penguinIt it = penguins.begin(); it != penguins.end(); ++it) {
            Point address = voxelGrid.getVoxelAddress(it->getPosRef(), lowCorner, highCorner);
            voxelGrid.updateContents(address, *it);
            it->setVoxelAddress(address);
        }

        for (penguinIt it = penguins.begin(); it != penguins.end(); ++it) {
            it->timeStep(bowl, voxelGrid, ignoreOthers);
        }

        eye.setPos(
            cameraDist * sin(cameraTheta),
            cameraDist * sin(cameraPhi),
            cameraDist * cos(cameraTheta)
        );
        look.setPos(0.0, 0.0, 0.0);
        up.setPos(0.0, 1.0, 0.0);

        cameraTheta += cameraThetaStep;
    }

    glutPostRedisplay();
    glutTimerFunc(timeStepSize, animation, id);
}

void reshape(int width, int height) {
    winWidth = width;
    winHeight = height;
    glViewport(0, 0, winWidth, winHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(viewAngle, (GLfloat) winWidth / (GLfloat) winHeight, 0.1, 2.5);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'z':
        viewAngle += 5;
        reshape(winWidth, winHeight);
        break;
    case 'Z':
        viewAngle -= 5;
        reshape(winWidth, winHeight);
        break;
    case 'R':
        initPenguins();
        break;
    case '+':
    case '=':
        for (penguinIt it = penguins.begin(); it != penguins.end(); ++it) {
            it->increaseDist();
        }
        penguins.begin()->printDists();
        break;
    case '-':
    case '_':
        for (penguinIt it = penguins.begin(); it != penguins.end(); ++it) {
            it->decreaseDist();
        }
        for (penguinIt it = penguins.begin(); it != penguins.end(); ++it) {
            penguins.begin()->printDists();
        }
        break;
    case 'p':
        isPaused = !isPaused;
        break;
    case 'c':
    case 'C':
        isPenguin = !isPenguin;
        break;
    case 'i':
    case 'I':
        ignoreOthers = !ignoreOthers;
        break;
    case 'v':
    case 'V':
        iView++;
        iView =  iView % nViewpointPenguins;
        break;
    case 27:
    case 'q':
    case 'Q':
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void key_special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        cameraPhi += cameraPhiStep;
        break;
    case GLUT_KEY_DOWN:
        cameraPhi -= cameraPhiStep;
        break;
    case GLUT_KEY_RIGHT:
        cameraThetaStep -= cameraThetaStepDelta;
        break;
    case GLUT_KEY_LEFT:
        cameraThetaStep += cameraThetaStepDelta;
        break;
    default:
        break;
    }
}

void initGlut() {
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(key_special);
    glutTimerFunc(timeStepSize, animation, 0);
}

int main(int argc, char** argv) {
    bowl = Bowl();
    voxelGrid = VoxelGrid();

    initPenguins();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    initGl();
    initGlut();

    glutMainLoop();
    return 0;
}
