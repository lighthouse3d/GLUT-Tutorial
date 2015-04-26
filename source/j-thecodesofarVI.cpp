#include <stdio.h>
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
// when no key is being pressesed
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

// color for the snowman's nose
float red = 1.0f, blue=0.5f, green=0.5f;

// scale of snowman
float scale = 1.0f;

// default font
void *font = GLUT_STROKE_ROMAN;

// width and height of the window
int h,w;

// variables to compute frames per second
int frame;
long time, timebase;
char s[60];
char currentMode[80];

// this string keeps the last good setting
// for the game mode
char gameModeString[40] = "640x480";

void init();

void changeSize(int ww, int hh) {

	h = hh;
	w = ww;
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

void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {

	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void renderStrokeFontString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {  

	char *c;
	glPushMatrix();
	glTranslatef(x, y,z);
	glScalef(0.002f, 0.002f, 0.002f);
	for (c=string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
	glPopMatrix();
}

void restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, w, h, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
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
	char number[3];
	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0f, 0.0f, j * 10.0f);
			drawSnowMan();
			sprintf(number,"%d",(i+3)*6+(j+3));
			renderStrokeFontString(0.0f, 0.5f, 0.0f, (void *)font ,number);
			glPopMatrix();
		}

	// Code to compute frames per second
	frame++;

	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s,"Lighthouse3D - FPS:%4.2f",
			frame*1000.0/(time-timebase));
		timebase = time;
		frame = 0;
	}

	setOrthographicProjection();
	void *font= GLUT_BITMAP_8_BY_13;
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(30,15,0,font,(char *)"GLUT Tutorial @ Lighthouse3D");
	renderBitmapString(30,30,0,font,s);
	renderBitmapString(30,45,0,font,(char *)"F1 - Game Mode  640x480 32 bits");
	renderBitmapString(30,60,0,font,(char *)"F2 - Game Mode  800x600 32 bits");
	renderBitmapString(30,75,0,font,(char *)"F3 - Game Mode 1024x768 32 bits");
	renderBitmapString(30,90,0,font,(char *)"F4 - Game Mode 1280x1024 32 bits");
	renderBitmapString(30,105,0,font,(char *)"F5 - Game Mode 1920x1200 32 bits");
	renderBitmapString(30,120,0,font,(char *)"F6 - Window Mode");
	renderBitmapString(30,135,0,font,(char *)"Esc - Quit");
	renderBitmapString(30,150,0,font,currentMode);
	glPopMatrix();

	restorePerspectiveProjection();

	glutSwapBuffers();
}

// -----------------------------------
//             KEYBOARD
// -----------------------------------

void processNormalKeys(unsigned char key, int xx, int yy) {

	switch (key) {
		case 27:
			if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0)
				glutLeaveGameMode();
			exit(0);
			break;
	}
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
		case GLUT_KEY_UP : deltaMove = 0.5f; break;
		case GLUT_KEY_DOWN : deltaMove = -0.5f; break;
		case GLUT_KEY_F1:
			// define resolution, color depth
			glutGameModeString("640x480:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"640x480:32");
				// register callbacks again
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F2:
			// define resolution, color depth
			glutGameModeString("800x600:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"800x600:32");
				// register callbacks again
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F3:
			// define resolution, color depth
			glutGameModeString("1024x768:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"1024x768:32");
				// register callbacks again
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F4:
			// define resolution, color depth
			glutGameModeString("1280x1024:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"1280x1024:32");
				// register callbacks again
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F5:
			// define resolution, color depth
			glutGameModeString("1920x1200");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"1920x1200");
				// register callbacks again
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F6:
			// return to default window
			w = 800;h = 600;
			if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0) {
				glutLeaveGameMode();
				//init();
			}
			break;
	}
	if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) == 0)
		sprintf(currentMode,"Current Mode: Window");
	else
		sprintf(currentMode,
			"Current Mode: Game Mode %dx%d at %d hertz, %d bpp",
			glutGameModeGet(GLUT_GAME_MODE_WIDTH),
			glutGameModeGet(GLUT_GAME_MODE_HEIGHT),
			glutGameModeGet(GLUT_GAME_MODE_REFRESH_RATE),
			glutGameModeGet(GLUT_GAME_MODE_PIXEL_DEPTH));
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

void init() {

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

// -----------------------------------
//             MAIN
// -----------------------------------

int main(int argc, char **argv) {

	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,600);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");

	// register callbacks
	init();

	// enter GLUT event processing cycle
	glutMainLoop();
	
	return 1;
}