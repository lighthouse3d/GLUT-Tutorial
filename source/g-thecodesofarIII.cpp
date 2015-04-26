#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// angle of rotation for the camera direction
float angle = 0.0f;

// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;

// XZ position of the camera
float x=0.0f, z=5.0f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

// Constant definitions for Menus
#define RED 1
#define GREEN 2
#define BLUE 3
#define ORANGE 4

#define FILL 1
#define LINE 2

#define SHRINK 1
#define NORMAL 2

// Pop up menu identifiers
int fillMenu, shrinkMenu, mainMenu, colorMenu;

// color for the nose
float red = 1.0f, blue=0.5f, green=0.5f;

// scale of snowman
float scale = 1.0f;

// menu status
int menuFlag = 0;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void drawSnowMan() {

	glScalef(scale, scale, scale);
	glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);

// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Nose
	glColor3f(red, green, blue);
	glRotatef(0.0f,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f,0.5f,10,2);

	glColor3f(1.0f, 1.0f, 1.0f);
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void renderScene(void) {

	if (deltaMove)
		computePos(deltaMove);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	x, 1.0f, z,
			x+lx, 1.0f,  z+lz,
			0.0f, 1.0f,  0.0f);

// Draw ground

	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();

// Draw 36 SnowMen

	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0f, 0.0f, j * 10.0f);
			drawSnowMan();
			glPopMatrix();
		}
	glutSwapBuffers();
}

// -----------------------------------
//             KEYBOARD
// -----------------------------------

void processNormalKeys(unsigned char key, int xx, int yy) {

	glutSetMenu(mainMenu);
	switch (key) {
		case 27:
			glutDestroyMenu(mainMenu);
			glutDestroyMenu(fillMenu);
			glutDestroyMenu(colorMenu);
			glutDestroyMenu(shrinkMenu);
			exit(0);
			break;

		case 's':
			if (!menuFlag)
			  glutChangeToSubMenu(2,"Shrink",shrinkMenu);
			break;
		case 'c':
			if (!menuFlag)
				glutChangeToSubMenu(2,"Color",colorMenu);
			break;
	}
	if (key == 27)
		exit(0);
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_UP : deltaMove = 0.5f; break;
		case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_UP :
		case GLUT_KEY_DOWN : deltaMove = 0;break;
	}
}

// -----------------------------------
//             MOUSE
// -----------------------------------

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			angle += deltaAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}

// -----------------------------------
//             MENUS
// -----------------------------------

void processMenuStatus(int status, int x, int y) {

	if (status == GLUT_MENU_IN_USE)
		menuFlag = 1;
	else
		menuFlag = 0;
}

void processMainMenu(int option) {

	// nothing to do in here
	// all actions are for submenus
}

void processFillMenu(int option) {

	switch (option) {

		case FILL: glPolygonMode(GL_FRONT, GL_FILL); break;
		case LINE: glPolygonMode(GL_FRONT, GL_LINE); break;
	}
}

void processShrinkMenu(int option) {

	switch (option) {

		case SHRINK: scale = 0.5f; break;
		case NORMAL: scale = 1.0f; break;
	}
}

void processColorMenu(int option) {

	switch (option) {
		case RED :
			red = 1.0f;
			green = 0.0f;
			blue = 0.0f; break;
		case GREEN :
			red = 0.0f;
			green = 1.0f;
			blue = 0.0f; break;
		case BLUE :
			red = 0.0f;
			green = 0.0f;
			blue = 1.0f; break;
		case ORANGE :
			red = 1.0f;
			green = 0.5f;
			blue = 0.5f; break;
	}
}

void createPopupMenus() {

	shrinkMenu = glutCreateMenu(processShrinkMenu);

	glutAddMenuEntry("Shrink",SHRINK);
	glutAddMenuEntry("NORMAL",NORMAL);

	fillMenu = glutCreateMenu(processFillMenu);

	glutAddMenuEntry("Fill",FILL);
	glutAddMenuEntry("Line",LINE);

	colorMenu = glutCreateMenu(processColorMenu);
	glutAddMenuEntry("Red",RED);
	glutAddMenuEntry("Blue",BLUE);
	glutAddMenuEntry("Green",GREEN);
	glutAddMenuEntry("Orange",ORANGE);

	mainMenu = glutCreateMenu(processMainMenu);

	glutAddSubMenu("Polygon Mode", fillMenu);
	glutAddSubMenu("Color", colorMenu);
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// this will allow us to know if the menu is active
	glutMenuStatusFunc(processMenuStatus);
}

// -----------------------------------
//             MAIN
// -----------------------------------

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// init Menus
	createPopupMenus();

	// enter GLUT event processing cycle
	glutMainLoop();
	
	return 1;
}