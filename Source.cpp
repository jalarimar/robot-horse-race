//#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <GL/freeglut.h>
using namespace std;

#define PI 3.14159265

GLUquadric *q;    //Required for creating cylindrical objects
double cam_height = 50;
double cam_rotation = 0;
double cam_position_x = 0;
double cam_position_z = 220;
double los_x = 0;
double los_z = -1;


				  //-- Ground Plane --------------------------------------------------------
void floor()
{
	float white[4] = { 1., 1., 1., 1. };
	float black[4] = { 0 };
	glColor4f(0.8, 0.72, 0.6, 1.0);  //The floor is dusty brown
	glNormal3f(0.0, 1.0, 0.0);

	//The floor is made up of several tiny squares on a 200x200 grid. Each square has a unit size.
	glBegin(GL_QUADS);
	for (int i = -200; i < 200; i++)
	{
		for (int j = -200; j < 200; j++)
		{
			glVertex3f(i, 0.0, j);
			glVertex3f(i, 0.0, j + 1);
			glVertex3f(i + 1, 0.0, j + 1);
			glVertex3f(i + 1, 0.0, j);
		}
	}
	glEnd();

}

//--Special keyboard event callback function ---------
void special(int key, int x, int y)
{
	float speed = 2.0f;
	if (key == GLUT_KEY_UP) {
		cam_position_x += los_x * speed; // move camera forward in the current direction
		cam_position_z += los_z * speed;
	}
	else if (key == GLUT_KEY_DOWN) {
		cam_position_x -= los_x * speed; // move camera backward in the current direction, keeping the look direction same
		cam_position_z -= los_z * speed;
	}
	else if (key == GLUT_KEY_LEFT) cam_rotation -= 5; // turn left by 5 degs
	else if (key == GLUT_KEY_RIGHT) cam_rotation += 5; // turn right by 5 degs
	los_x = sin(cam_rotation * PI / 180.0);
	los_z = -cos(cam_rotation * PI / 180.0);
	cout << "pos: " << cam_position_x << " " << cam_position_z << " dir: " << los_x << " " << los_z << endl;
	glutPostRedisplay();
}

void initialize(void)
{
	float grey[4] = { 0.2, 0.2, 0.2, 1.0 };
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };

	q = gluNewQuadric();

	//	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	gluQuadricDrawStyle(q, GLU_FILL);
	glClearColor(0.0, 0.0, 0.0, 0.0);  //Background colour

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60., 1.0, 1.0, 1000.0);   //Perspective projection
}

void display(void)
{
	float lgt_pos[] = { 0.0f, 50.0f, 0.0f, 1.0f };  //light0 position (directly above the origin)

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		cam_position_x, cam_height, cam_position_z,
		cam_position_x + los_x, cam_height - 0.25, cam_position_z + los_z,
		0.0, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position

	floor();

	glutSwapBuffers();   //Useful for animation
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Wild West");
	glutSpecialFunc(special);
	initialize();

	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}