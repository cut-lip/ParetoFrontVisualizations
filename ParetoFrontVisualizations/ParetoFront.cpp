#include "GL/glut.h"
#include "Canvas.h"
#include "myGLlib.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

const unsigned int SCREEN_WIDTH = 640;
const unsigned int SCREEN_HEIGHT = 200;

Point2 CP = Point2(0, 0);
float CD = 0.0;

// Draw a line from CP to the new vertex
void lineTo(float x, float y)
{
	glBegin(GL_LINES);
	glVertex2f((GLfloat)CP.getX(), (GLfloat)CP.getY());
	glVertex2f((GLfloat)x, (GLfloat)y);
	glEnd();

	CP.set(x, y);	// Update the CP
	glFlush();		// Flush the buffer
}

void lineTo(Point2 p)
{
	glBegin(GL_LINES);
	glVertex2f((GLfloat)CP.getX(), (GLfloat)CP.getY());
	glVertex2f((GLfloat)p.getX(), (GLfloat)p.getY());
	glEnd();

	CP.set(p.getX(), p.getY());	// Update the CP
	glFlush();		// Flush the buffer
}

// Update the CP
void moveTo(float x, float y)
{
	CP.set(x, y);
}

void moveTo(Point2 p)
{
	CP.set(p.getX(), p.getY());
}

// TURTLE GRAPHICS
// turn the turtle to given angle
void turnTo(float angle)
{
	// Convert radians to degrees
	CD = angle * (180.0 / 3.141592653589793238463);
}

// turn the turtle given number of degrees
void turn(float angle)
{
	// Convert radians to degrees
	CD += angle * (180.0 / 3.141592653589793238463);
}

// move turtle forward in a straight line from CP
void forward(float dist, bool isVisible)
{
	const float radPerDeg = 0.017453393;
	float x = CP.getX() + dist * cos(radPerDeg * CD);
	float y = CP.getY() + dist * sin(radPerDeg * CD);

	// Move CP
	if (isVisible)
		lineTo(x, y);
	else
		moveTo(x, y);
}

void myDisplay()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT);

	/*
	// Draw horizontal axis
	glLineWidth(4.0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex2d(0.0, 20.0);
	glVertex2d(SCREEN_WIDTH, 20.0);
	glEnd();
	*/

	int scaleFactor = 10;

	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 1.0);

	std::vector<std::vector<double>> gradeData{
		//Real grades
		{1.0, 2.0, 3.4, 2.0, 1.0},
		{1.5, 1.8, 3.8, 2.4, 2.0},
		{2.0, 3.0, 2.0, 4.0, 3.0},
		{4.0, 2.5, 1.0, 3.0, 4.0},
		{2.5, 2.0, 3.0, 4.0, 2.5},
		{3.5, 2.5, 4.0, 2.0, 3.0},
	};

	std::vector<double> gradeWeights{ 0.2, 0.4, 0.8, 1.0, 0.4 };
	double  MAX_WEIGHT = 1.0;

	// Compute set of values k from weights
	std::vector<double> constantsK{};
	for (double d : gradeWeights)
	{
		constantsK.push_back(d / MAX_WEIGHT);
	}

	// Use grade weights to determine angles for GLC-L visualization
	std::vector<double> angles;

	
	// Normalize data
	double minValue = 1.0;
	double maxValue = 4.0;
	for (std::vector<double> vec : gradeData)
	{
		std::vector<double>::iterator iter = vec.begin();
		for (iter; iter < vec.end(); iter++)
		{
			*iter = (*iter - minValue) / (maxValue - minValue);
		}
	}
	

	for (double d : constantsK)
	{
		// REMEMBER TO IMPLEMENT CORRECT GLC-L ALGORITHM!!!!
		// (don't have to use absolute value, as I did)
		angles.push_back(acos(d));
	}

	// Pointer to the vectors of grades for each student
	//std::vector<std::vector<double>>::iterator gradeIt = gradeData.begin();

	// Graph the GLC-L visualization of each data point
	for (std::vector<double> student : gradeData)
	{
		//std::vector<double> student = *gradeIt;
		std::vector<double>::iterator studentIt = student.begin();

		std::vector<double>::iterator anglesIt = angles.begin();

		for (studentIt; studentIt < student.end(); studentIt++, anglesIt++)
		{
			// Use turtle graphics to draw GLC-L graph
			turnTo(*anglesIt);
			glPushMatrix();

			// Since everything is being transformed through the modelview matrix,
			// I want to scale it up to a reasonable scale
			// 
			//REMEMBER: translate before scale
			glTranslatef(50, 0, 0);
			glScalef(50.0, 13.0, 1.0);
			forward(*studentIt, true);
			glPopMatrix();
		}
		CP = Point2(0, 0);
		//gradeIt++;

	}

	//Ideal Student
	std::vector<double> ideal{ 4.0, 4.0, 4.0, 4.0, 4.0 };
	
	//std::vector<double> student = *gradeIt;
	std::vector<double>::iterator idealIt = ideal.begin();

	std::vector<double>::iterator anglesIt = angles.begin();

	glColor3f(0.0, 1.0, 0.0);
	for (idealIt; idealIt < ideal.end(); idealIt++, anglesIt++)
	{
		// Use turtle graphics to draw GLC-L graph
		turnTo(*anglesIt);
		glPushMatrix();

		// Since everything is being transformed through the modelview matrix,
		// I want to scale it up to a reasonable scale
		glTranslatef(50, 0, 0);
		glScalef(50.0, 13.0, 1.0);
		//glTranslatef(-250, 50, 0);
		forward(*idealIt, true);
		glPopMatrix();
	}

	
	glFlush();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GLC-L");
	glutDisplayFunc(myDisplay);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glutMainLoop();
}