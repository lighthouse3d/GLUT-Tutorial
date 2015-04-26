
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define n 5

void renderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glScalef(0.2, 0.2, 0.2);

	//glScalef(0.5, 0.5, 0.5);

	//glTranslatef(-n-1, 0, 0);
	//for (int i = 0; i < n; i++) {
	//	glRotatef(45,0,0,1);
	//	glutWireCube(2);
	//	glRotatef(-45,0,0,1);
	//	glTranslatef(2.82,0,0);
	//}

	//for (int i = 0; i < n; i++) {
	//
	//	glPushMatrix();

	//	glTranslatef(1.414 *(- n*0.5 + i + 0.5), 0, 0);
	//	glRotatef(45, 0 ,0 ,1);
	//	glScalef(0.5, 0.5, 0.5);
	//	glutWireCube(2);

	//	glPopMatrix();
	//
	//	glPushMatrix();

	//	glTranslatef(0, 1.414 *(- n*0.5 + i + 0.5), 0);
	//	glRotatef(45, 0 ,0 ,1);
	//	glScalef(0.5, 0.5, 0.5);
	//	glutWireCube(2);

	//	glPopMatrix();
	//}



	glBegin(GL_TRIANGLES);
		glVertex3f(-0.5,-0.5,0.0);
		glVertex3f(0.5,0.0,0.0);
		glVertex3f(0.0,0.5,0.0);
	glEnd();

        glutSwapBuffers();
}

int main(int argc, char **argv) {

	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Lighthouse3D - GLUT Tutorial");

	// register callbacks
	glutDisplayFunc(renderScene);

	// enter GLUT event processing cycle
	glutMainLoop();
	
	return 1;
}