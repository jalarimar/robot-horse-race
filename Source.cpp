//#include <stdlib.h>
#include <iostream>
#include <string>
#include <math.h>
#include <GL/freeglut.h>
#include "loadBMP.h"
#include "loadTGA.h"
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
float blade_theta = 0;
float trophy_theta = 0;
float my_horse_theta = 0;
float your_horse_theta = 0;
float other_horse_theta = 0;
float front_leg_theta = 0;
float back_leg_theta = 0;
bool front_leg_fowards = false;
bool back_leg_fowards = true;

// colours
float white[4] = { 1.0, 1.0, 1.0, 1.0 };
float grey[4] = { 0.2, 0.2, 0.2, 1.0 };
float black[4] = { 0 };

// texture
GLuint txId[10]; 
int texCount = 0;

void makeTexture(char* name, bool isTGA)
{
	glBindTexture(GL_TEXTURE_2D, txId[texCount]);
	if (isTGA) {
		loadTGA(name);
	} else {
		loadBMP(name);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	texCount += 1;
}

//------Function to load a texture in bmp format  ------------------------
void loadTexture()
{
	glGenTextures(10, txId);
	
	makeTexture("RaceFlag.bmp", false);
	makeTexture("1stPlace.bmp", false);
	makeTexture("2ndPlace.bmp", false);
	makeTexture("3rdPlace.bmp", false);
	makeTexture("up.tga", true);
	makeTexture("down.tga", true);
	makeTexture("front.tga", true);
	makeTexture("back.tga", true);
	makeTexture("left.tga", true);
	makeTexture("right.tga", true);
}

				  //-- Ground Plane --------------------------------------------------------
void floor()
{
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

void skybox()
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);

	float x_left = -500;
	float x_right = 500;
	float y_top = 200;
	float y_bottom = -1;
	float z_back = -500;
	float z_front = 500;
	
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, txId[4]);
	glBegin(GL_QUADS);		
		// top
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(0, 1); glVertex3f(x_left, y_top, z_back);
		glTexCoord2f(0, 0); glVertex3f(x_right, y_top, z_back);
		glTexCoord2f(1, 0); glVertex3f(x_right, y_top, z_front);
		glTexCoord2f(1, 1); glVertex3f(x_left, y_top, z_front);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, txId[5]);
	glBegin(GL_QUADS);
		// bottom
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(1, 0); glVertex3f(x_right, y_bottom, z_back);
		glTexCoord2f(0, 0); glVertex3f(x_left, y_bottom, z_back);
		glTexCoord2f(0, 1); glVertex3f(x_left, y_bottom, z_front);
		glTexCoord2f(1, 1); glVertex3f(x_right, y_bottom, z_front);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, txId[7]);
	glBegin(GL_QUADS);
		// front
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0, 1); glVertex3f(x_left, y_top, z_back);
		glTexCoord2f(0, 0.3); glVertex3f(x_left, y_bottom, z_back);
		glTexCoord2f(1, 0.3); glVertex3f(x_right, y_bottom, z_back);
		glTexCoord2f(1, 1); glVertex3f(x_right, y_top, z_back);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, txId[6]);
	glBegin(GL_QUADS);
		// back
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(1, 0.3); glVertex3f(x_left, y_bottom, z_front);
		glTexCoord2f(1, 1); glVertex3f(x_left, y_top, z_front);
		glTexCoord2f(0, 1); glVertex3f(x_right, y_top, z_front);
		glTexCoord2f(0, 0.3); glVertex3f(x_right, y_bottom, z_front);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, txId[8]);
	glBegin(GL_QUADS);
		// left
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(0, 1); glVertex3f(x_left, y_top, z_front);
		glTexCoord2f(0, 0.3); glVertex3f(x_left, y_bottom, z_front);
		glTexCoord2f(1, 0.3); glVertex3f(x_left, y_bottom, z_back);
		glTexCoord2f(1, 1); glVertex3f(x_left, y_top, z_back);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, txId[9]);
	glBegin(GL_QUADS);
		// right
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(1, 0.3); glVertex3f(x_right, y_bottom, z_front);
		glTexCoord2f(1, 1); glVertex3f(x_right, y_top, z_front);
		glTexCoord2f(0, 1); glVertex3f(x_right, y_top, z_back);
		glTexCoord2f(0, 0.3); glVertex3f(x_right, y_bottom, z_back);
	glEnd();

	glDisable(GL_TEXTURE_2D);
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

void block(float x_min, float x_max, float y_min, float y_max, float z_min, float z_max, GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0); //Facing +z
		glTexCoord2f(0, 0); glVertex3f(x_min, y_min, z_max);
		glTexCoord2f(1, 0); glVertex3f(x_max, y_min, z_max);
		glTexCoord2f(1, 1); glVertex3f(x_max, y_max, z_max);
		glTexCoord2f(0, 1); glVertex3f(x_min, y_max, z_max);

		glNormal3f(0.0, 0.0, -1.0); //Facing -z
		glTexCoord2f(0, 0); glVertex3f(x_max, y_min, z_min);
		glTexCoord2f(1, 0); glVertex3f(x_min, y_min, z_min);
		glTexCoord2f(1, 1); glVertex3f(x_min, y_max, z_min);
		glTexCoord2f(0, 1); glVertex3f(x_max, y_max, z_min);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
		glNormal3f(1.0, 0.0, 0.0); //Facing +x
		glVertex3f(x_max, y_min, z_max);
		glVertex3f(x_max, y_min, z_min);
		glVertex3f(x_max, y_max, z_min);
		glVertex3f(x_max, y_max, z_max);

		glNormal3f(-1.0, 0.0, 0.0); //Facing -x
		glVertex3f(x_min, y_min, z_max);
		glVertex3f(x_min, y_min, z_min);
		glVertex3f(x_min, y_max, z_min);
		glVertex3f(x_min, y_max, z_max);

		glNormal3f(0.0, 1.0, 0.0); //Facing +y
		glVertex3f(x_min, y_max, z_min);
		glVertex3f(x_max, y_max, z_min);
		glVertex3f(x_max, y_max, z_max);
		glVertex3f(x_min, y_max, z_max);

	glEnd();
}

void podium()
{
	glColor4f(0.15, 0.15, 0.15, 1.0); // near black
	block(-8, 8, 0, 22, -7.5, 7.5, txId[1]);
	block(-24, -8, 0, 15, -7.5, 7.5, txId[2]);
	block(8, 24, 0, 10, -7.5, 7.5, txId[3]);
	glColor4f(0.3, 0.2, 0.2, 1.0); // dark brown
}

void actual_trophy()
{
	// teapot
	glPushMatrix();
		glTranslatef(0, 10, 0);
		glutSolidTeapot(3);
	glPopMatrix();

	// cylinder
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		gluCylinder(q, 1, 3, 9, 10, 1);
	glPopMatrix();

	// cube
	glPushMatrix();
		glScalef(2, 1, 2);
		glutSolidCube(2);
	glPopMatrix();
}

void turntable()
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);
	glPushMatrix();
		glRotatef(-90, 1, 0, 0);
		gluCylinder(q, 10, 10, 3, 20, 1);
		glTranslatef(0, 0, 3);
		gluDisk(q, 0, 10, 20, 1);		
	glPopMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
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
			glColor4f(0, 0.9, 0.9, 1.0); // cyan lampshade
			gluCylinder(q, 0.5, 3, 3, 20, 1);
			glTranslatef(0, 0, 3);
			glColor4f(1, 1, 0.8, 1.0); // yellow-white light
			gluDisk(q, 0, 3, 20, 1);
			glColor4f(0.3, 0.2, 0.2, 1.0); // dark brown post
		glPopMatrix();

		glTranslatef(0, 13, 0);
		glScalef(1, 26, 1);
		glutSolidCube(1);
	glPopMatrix();

}

void trophy()
{
	glPushMatrix();
		glTranslatef(65, 0, 0);
		//lamp();
		
		
		glPushMatrix();
			glRotatef(trophy_theta, 0, 1, 0);
			glTranslatef(0, 5, 0);
			glScalef(1.25, 1.25, 1.25);
			glColor4f(0.7, 0.6, 0.1, 1.0); // golden trophy
			actual_trophy();
			glColor4f(0.3, 0.2, 0.2, 1.0); // dark brown
		glPopMatrix();
		
		// draw trophy shadow
		float lx = -70; //30
		float ly = 100; //40
		float lz = -50; //-15
		
		glDisable(GL_LIGHTING);
		
		float shadowMat[16] = { ly,0.5,0,0,	-lx,0.5,-lz,-1,
							0,0.5,ly,0,	0,0.5,0,ly };
	
		glPushMatrix();
			glMultMatrixf(shadowMat);
			glRotatef(trophy_theta, 0, 1, 0);
			glTranslatef(0, 5, 0);
			glScalef(1.25, 1.25, 1.25);
			glColor4f(0.2, 0.2, 0.2, 0.3); // dark grey
			actual_trophy();
		glPopMatrix();
		
		glEnable(GL_LIGHTING);
		glColor4f(0.3, 0.2, 0.2, 1.0); // dark brown
		
		glRotatef(trophy_theta, 0, 1, 0);
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
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	
	glBegin(GL_QUADS);
		glNormal3f(1.0, 0.0, 0.0); //Facing +x
		glTexCoord2f(0, 0); glVertex3f(0, 20, -29.0);
		glTexCoord2f(1.5, 0); glVertex3f(0, 20, 29.0);
		glTexCoord2f(1.5, 0.7); glVertex3f(0, 30, 29.0);
		glTexCoord2f(0, 0.65); glVertex3f(0, 30, -29.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
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
		glColor4f(0.7, 0, 0, 1.0); // red
		glRotatef(your_horse_theta, 0, 1, 0);
		glTranslatef(0, 0, 105);
		front_left_leg();
		front_right_leg();
		back_left_leg();
		back_right_leg();
		body();
		glColor4f(0.3, 0.2, 0.2, 1.0); // dark brown
	glPopMatrix();
}

void my_horse()
{
	glPushMatrix();
		glColor4f(0.1, 0.7, 0.9, 1.0); // blue
		glRotatef(my_horse_theta, 0, 1, 0);
		glTranslatef(0, 0, 125);
		front_left_leg();
		front_right_leg();
		back_left_leg();
		back_right_leg();
		body();
		glColor4f(0.3, 0.2, 0.2, 1.0); // dark brown
	glPopMatrix();
}

void other_horse()
{
	glPushMatrix();
		glColor4f(0, 1, 0.2, 1.0); // green
		glRotatef(other_horse_theta, 0, 1, 0);
		glTranslatef(0, 0, 145);
		front_left_leg();
		front_right_leg();
		back_left_leg();
		back_right_leg();
		body();
		glColor4f(0.3, 0.2, 0.2, 1.0); // dark brown
	glPopMatrix();
}

void camera()
{
	glPushMatrix();
		glRotatef(15, 1, 0, 0);
		gluCylinder(q, 1, 3, 10, 20, 1);
		glTranslatef(0, 0, 10);
		glColor4f(1, 1, 0.6, 1.0); // yellow-white light
		gluDisk(q, 0, 3, 20, 1);
		glColor4f(0.1, 0.1, 0.1, 1.0); // dark brown post
	glPopMatrix();
}

void blade()
{
	glBegin(GL_TRIANGLES);
		glVertex3f(0, 7.5, 0);
		glVertex3f(6, 7.5, 0);
		glVertex3f(6, 7.5, 8);
	glEnd();
}

void stick()
{
	glPushMatrix();
		glTranslatef(0, 4, 1.5);
		glScalef(1, 8, 1);
		glutSolidCube(1);
	glPopMatrix();
}

void drone()
{
	glPushMatrix();
		glColor4f(0.1, 0.1, 0.1, 1.0); // black camera
		glRotatef(my_horse_theta, 0, 1, 0); // follow horse
		glTranslatef(0, 35, 125); // above my horse
		glRotatef(90, 0, 1, 0); // face same direction as my horse
		camera();
		glTranslatef(0, 0, 4);
		stick();
		glRotatef(10, 1, 0, 0);
		glRotatef(blade_theta, 0, 1, 0);
		blade();
		glRotatef(90, 0, 1, 0);
		blade();
		glRotatef(180, 0, 1, 0);
		blade();
		glRotatef(-90, 0, 1, 0);
		blade();
		glColor4f(0.3, 0.2, 0.2, 1.0); // black camera
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

	float max_pos = 200;
	float min_pos = -200;
	if (cam_position_x > max_pos) {
		cam_position_x = max_pos;
	} else if (cam_position_x < min_pos) {
		cam_position_x = min_pos;
	}
	if (cam_position_z > max_pos) {
		cam_position_z = max_pos;
	}
	else if (cam_position_z < min_pos) {
		cam_position_z = min_pos;
	}

	glutPostRedisplay();
}

void initialize(void)
{
	q = gluNewQuadric();

	loadTexture();

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
	float lgt_pos[] = { -70.0f, 100.0f, -50.0f, 1.0f };  //light0 position
	float spotlgt_pos[] = { 65.0f, 20.0f, -15.0f, 1.0f }; // light1 position
	float spotdir[] = { -5.0, -3.0, 4.0 };

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
	
	glDisable(GL_LIGHT1);

	// inverse transform??
	glRotatef(90, 0, 1, 0);
	glTranslatef(-230, 15, -75);
	glRotatef(-my_horse_theta, 0, 1, 0);

	floor();
	skybox();
	tracks();
	podium();
	trophy();
	finishline();
	your_horse();
	my_horse();
	other_horse();
	drone();

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
	your_horse_theta += 1.02;
	other_horse_theta += 0.97;
	trophy_theta++;
	blade_theta -= 10;

	// TODO moving spotlight & camera
	
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
	glutCreateWindow("Wild West Robot Race");
	glutSpecialFunc(special);
	glutTimerFunc(20, myTimer, 0);
	initialize();

	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
