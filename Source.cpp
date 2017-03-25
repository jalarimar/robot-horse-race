//#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <GL/freeglut.h>
using namespace std;

#define PI 3.14159265

GLUquadric *q;    //Required for creating cylindrical objects

// camera movement
double cam_height = 50;
double cam_rotation = 0;
double cam_position_x = 50;
double cam_position_z = 220;
double los_x = 0;
double los_z = -1;

// rotation
float trophy_theta = 0;
float my_horse_theta = 0;
float your_horse_theta = 0;
float front_leg_theta = 0;
float back_leg_theta = 0;
bool front_leg_fowards = false;
bool back_leg_fowards = true;


				  //-- Ground Plane --------------------------------------------------------
void floor()
{
	float white[4] = { 1., 1., 1., 1. };
	float black[4] = { 0 };
	glColor4f(0.8, 0.72, 0.6, 1.0);  //The floor is dusty brown
	glNormal3f(0.0, 1.0, 0.0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);

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

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
}

void track(float radius)
{
	float angle1, angle2, ca1, sa1, ca2, sa2;
	float x1, z1, x2, z2, x3, z3, x4, z4;  //four points of a quad
	float toRad = 3.14159265 / 180.0;  //Conversion from degrees to radians

	glBegin(GL_QUADS);
	for (int i = 0; i < 360; i += 5)    //5 deg intervals
	{
		angle1 = i * toRad;       //Computation of angles, cos, sin etc
		angle2 = (i + 5) * toRad;
		ca1 = cos(angle1); ca2 = cos(angle2);
		sa1 = sin(angle1); sa2 = sin(angle2);
		x1 = (radius - 0.5)*sa1; z1 = (radius - 0.5)*ca1;
		x2 = (radius + 0.5)*sa1; z2 = (radius + 0.5)*ca1;
		x3 = (radius + 0.5)*sa2; z3 = (radius + 0.5)*ca2;
		x4 = (radius - 0.5)*sa2; z4 = (radius - 0.5)*ca2;

		glNormal3f(0., 1., 0.);       //Quad 1 facing up
		glVertex3f(x1, 1.0, z1);
		glVertex3f(x2, 1.0, z2);
		glVertex3f(x3, 1.0, z3);
		glVertex3f(x4, 1.0, z4);

		glNormal3f(-sa1, 0.0, -ca1);   //Quad 2 facing inward
		glVertex3f(x1, 0.0, z1);
		glVertex3f(x1, 1.0, z1);
		glNormal3f(-sa2, 0.0, -ca2);
		glVertex3f(x4, 1.0, z4);
		glVertex3f(x4, 0.0, z4);

		glNormal3f(sa1, 0.0, ca1);   //Quad 3 facing outward
		glVertex3f(x2, 1.0, z2);
		glVertex3f(x2, 0.0, z2);
		glNormal3f(sa2, 0.0, ca2);
		glVertex3f(x3, 0.0, z3);
		glVertex3f(x3, 1.0, z3);
	}
	glEnd();
}

void tracks()
{
	glColor4f(0.3, 0.2, 0.2, 1.0); // dark brown track boundaries
	track(95.0);   // Inner track boundary has radius 95 units
	track(155.0);   //Outer track boundary has radius 155 units
}

void firstblock()
{
	glPushMatrix();
		glScalef(1, 3, 1);
		glutSolidCube(15);
	glPopMatrix();
}

void secondblock() 
{
	glPushMatrix();
		glTranslatef(-15, 0, 0);
		glScalef(1, 2, 1);
		glutSolidCube(15);
	glPopMatrix();
}

void thirdblock()
{
	glPushMatrix();
		glTranslatef(15, 0, 0);
		glScalef(1, 1, 1);
		glutSolidCube(15);
	glPopMatrix();
}

void podium()
{
	firstblock();
	secondblock();
	thirdblock();
}

void trophy_base()
{
	// teapot
	glPushMatrix();
		glTranslatef(0, 10, 0);
		glutSolidTeapot(3);
	glPopMatrix();

	// cylinder
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		gluCylinder(q, 1, 3, 10, 10, 10);
	glPopMatrix();

	// cube
	glPushMatrix();
		glScalef(2, 1, 2);
		glutSolidCube(2);
	glPopMatrix();
}

void turntable()
{
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		gluCylinder(q, 10, 10, 3, 20, 1);
		glTranslatef(0, 0, 3);
		gluDisk(q, 0, 10, 20, 1);
		
	glPopMatrix();
}

void lamp()
{
	// post
	glPushMatrix();
		glTranslatef(15, 0, -15);

		// lamp
		glPushMatrix();
			glRotatef(-45, 0, 1, 0);
			glTranslatef(0, 25, 0);
			glRotatef(25, 1, 0, 0);
			gluCylinder(q, 1, 3, 3, 20, 1);
			glTranslatef(0, 0, 3);
			glColor4f(1, 1, 0.8, 1.0); 
			gluDisk(q, 0, 3, 20, 1);
			glColor4f(0.3, 0.2, 0.2, 1.0);
		glPopMatrix();

		glScalef(1, 50, 1);
		glutSolidCube(1);
	glPopMatrix();

}

void trophy()
{
	glPushMatrix();
		glTranslatef(50, 0, 0);
		lamp();
		glRotatef(trophy_theta, 0, 1, 0);
		glPushMatrix();
			glTranslatef(0, 5, 0);
			glScalef(1.25, 1.25, 1.25);
			trophy_base();
		glPopMatrix();
		turntable();
		
	glPopMatrix();
}

void innerpost()
{
	glPushMatrix();
		glTranslatef(0, 0, -30);
		glScalef(1, 30, 1);
		glutSolidCube(2);
	glPopMatrix();
}

void outerpost()
{
	glPushMatrix();
		glTranslatef(0, 0, 30);
		glScalef(1, 30, 1);
		glutSolidCube(2);
	glPopMatrix();
}

void banner()
{
	glPushMatrix();
		glTranslatef(0, 25, 0);
		glScalef(1, 5, 30);
		glutSolidCube(2);
	glPopMatrix();
}

void finishline()
{
	glPushMatrix();
		glTranslatef(0, 0, 125);
		innerpost();
		outerpost();
		banner();
	glPopMatrix();
}

void front_left_leg()
{
	glPushMatrix();
		glTranslatef(5, 10, -2);
		glRotatef(front_leg_theta, 0, 0, 1);
		glTranslatef(0, -5, 0);
		glScalef(1, 10, 1);
		glutSolidCube(1);
	glPopMatrix();
}

void front_right_leg()
{
	glPushMatrix();
		glTranslatef(5, 10, 2);
		glRotatef(front_leg_theta, 0, 0, 1);
		glTranslatef(0, -5, 0);
		glScalef(1, 10, 1);
		glutSolidCube(1);
	glPopMatrix();
}

void back_left_leg()
{
	glPushMatrix();
		glTranslatef(-5, 10, -2);
		glRotatef(back_leg_theta, 0, 0, 1);
		glTranslatef(0, -5, 0);
		glScalef(1, 10, 1);
		glutSolidCube(1);
	glPopMatrix();
}

void back_right_leg()
{
	glPushMatrix();
		glTranslatef(-5, 10, 2);
		glRotatef(back_leg_theta, 0, 0, 1);
		glTranslatef(0, -5, 0);
		glScalef(1, 10, 1);
		glutSolidCube(1);
	glPopMatrix();
}

void tail()
{
	glPushMatrix();
		glTranslatef(-7, 0, 0);
		glRotatef(25, 0, 0, 1);
		glScalef(15, 1, 1);
		glutSolidCube(0.5);
	glPopMatrix();
}

void head()
{
	glPushMatrix();
		glTranslatef(4, -2, 0);
		glRotatef(20, 0, 0, 1);
		glScalef(2, 5, 3);
		glutSolidCube(1);
	glPopMatrix();
}

void neck()
{
	glPushMatrix();
		glTranslatef(6, 3, 0);
		glRotatef(40, 0, 0, 1);
		head();
		glScalef(8, 1, 2);
		glutSolidCube(1);
	glPopMatrix();
}

void body()
{
	glPushMatrix();
		glTranslatef(0, 10, 0);
		tail();
		neck();
		glScalef(12, 4, 6);
		glutSolidCube(1);
	glPopMatrix();
}

void your_horse()
{
	glPushMatrix();
		glRotatef(your_horse_theta, 0, 1, 0);
		glTranslatef(0, 0, 115);
		front_left_leg();
		front_right_leg();
		back_left_leg();
		back_right_leg();
		body();
	glPopMatrix();
}

void my_horse()
{
	glPushMatrix();
		glRotatef(my_horse_theta, 0, 1, 0);
		glTranslatef(0, 0, 135);
		front_left_leg();
		front_right_leg();
		back_left_leg();
		back_right_leg();
		body();
	glPopMatrix();
}

//--Special keyboard event callback function ---------
void special(int key, int x, int y)
{
	float speed = 8.0f;
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
	// cout << "pos: " << cam_position_x << " " << cam_position_z << " dir: " << los_x << " " << los_z << endl;
	glutPostRedisplay();
}

void initialize(void)
{
	float grey[4] = { 0.2, 0.2, 0.2, 1.0 };
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };

	q = gluNewQuadric();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);

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
	float lgt_pos[] = { 0.0f, 100.0f, 175.0f, 1.0f };  //light0 position (directly above the origin)
	float spotlgt_pos[] = { 65.0f, 25.0f, -15.0f, 1.0f }; // light1 position
	float spotdir[] = { -5.0, -7.0, 4.0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		cam_position_x, cam_height, cam_position_z,
		cam_position_x + los_x, cam_height - 0.25, cam_position_z + los_z,
		0.0, 1.0, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position
	glLightfv(GL_LIGHT1, GL_POSITION, spotlgt_pos);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir);

	floor();
	tracks();
	podium();
	trophy();
	finishline();
	your_horse();
	my_horse();

	glutSwapBuffers();   //Useful for animation
}


void calculateLegMovement() {
	// front leg angle
	if (front_leg_theta > 30) {
		front_leg_fowards = false;
		front_leg_theta = 30;
	}
	else if (front_leg_theta < -30) {
		front_leg_fowards = true;
		front_leg_theta = -30;
	}
	else if (front_leg_fowards) {
		front_leg_theta += 5;
	}
	else {
		front_leg_theta -= 5;
	}

	// back leg angle
	if (back_leg_theta > 30) {
		back_leg_fowards = false;
		back_leg_theta = 30;
	}
	else if (back_leg_theta < -30) {
		back_leg_fowards = true;
		back_leg_theta = -30;
	}
	else if (back_leg_fowards) {
		back_leg_theta += 5;
	}
	else {
		back_leg_theta -= 5;
	}
}

void myTimer(int value)
{
	my_horse_theta++;
	your_horse_theta += 1.05;
	trophy_theta++;
	
	calculateLegMovement();


	glutPostRedisplay();
	glutTimerFunc(20, myTimer, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Wild West");
	glutSpecialFunc(special);
	glutTimerFunc(40, myTimer, 0);
	initialize();

	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}