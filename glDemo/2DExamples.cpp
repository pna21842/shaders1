
#include "2DExamples.h"

void render2D_triangle() {

	glBegin(GL_TRIANGLES);

	glColor3ub(255, 0, 0);
	glVertex2f(-0.5f, -0.5f);
	
	glColor3ub(0, 255, 0);
	glVertex2f(0.0f, 0.5f);

	glColor3ub(0, 0, 255);
	glVertex2f(0.5f, -0.5f);

	glEnd();
}

void render2D_quadOutline() {

	glBegin(GL_LINE_LOOP);

	glColor3ub(255, 255, 255);

	glVertex2f(-1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(1.0f, -1.0f);
	glVertex2f(-1.0f, -1.0f);

	glEnd();
}

void render2D_star() {

	glBegin(GL_LINE_LOOP);

	glColor3ub(255, 0, 0);
	glVertex2f(0.0, 0.25f);

	glColor3ub(255, 255, 0);
	glVertex2f(0.1f, 0.1f);
	
	glColor3ub(0, 255, 0);
	glVertex2f(0.25f, 0.08f);
	
	glColor3ub(0, 255, 255);
	glVertex2f(0.15f, -0.05f);
	
	glColor3ub(0, 0, 255);
	glVertex2f(0.25f, -0.25f);
	
	glColor3ub(128, 45, 200);
	glVertex2f(0.0, -0.125f);
	
	glColor3ub(0, 255, 0);
	glVertex2f(-0.25f, -0.25f);
	
	glColor3ub(255, 0, 255);
	glVertex2f(-0.15f, -0.05f);
	
	glColor3ub(255, 128, 255);
	glVertex2f(-0.25f, 0.08f);
	
	glColor3ub(255, 128, 128);
	glVertex2f(-0.1f, 0.1f);

	glEnd();
}
