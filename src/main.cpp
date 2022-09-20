#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

struct Point {
	GLint x;
	GLint y;
};

Point p1, p2;

void drawLine(Point p1, Point p2) {
	auto lineLow = [](int x0, int y0, int x1, int y1) 
	{ 
		GLfloat dx = x1 - x0;
		GLfloat dy = y1 - y0;
		GLfloat yi = 1;
		if (dy < 0) {
			yi = -1;
			dy = -dy;
		}
		GLfloat D = 2 * dy - dx;
		GLfloat y = y0;

		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1, 1, 1);
		glBegin(GL_POINTS);
		for (int x = x0; x < x1; x++) {
			glVertex2i(x, y);
			if (D > 0) {
				y += yi;
				D += 2 * (dy - dx);
			}
			else
				D += 2 * dy;
		}
		glEnd();
		glFlush();
	};

	auto lineHigh = [](int x0, int y0, int x1, int y1) { 
		GLfloat dx = x1 - x0;
		GLfloat dy = y1 - y0;
		GLfloat xi = 1;
		if (dx < 0) {
			xi = -1;
			dx = -dx;
		}
		GLfloat D = 2 * dy - dx;
		GLfloat x = x0;

		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1, 1, 1);
		glBegin(GL_POINTS);
		for (int y = y0; y < y1; y++) {
			glVertex2i(x, y);
			if (D > 0) {
				x += xi;
				D += 2 * (dx - dy);
			}
			else
				D += 2 * dx;
		}
		glEnd();
		glFlush();
	};

	if (abs(p2.y - p1.y) < abs(p2.x - p1.x)) {
		if (p1.x > p2.x) lineLow(p2.x, p2.y, p1.x, p1.y);
		else lineLow(p1.x, p1.y, p2.x, p2.y);
	}
	else {
		if (p1.y > p2.y) lineHigh(p2.x, p2.y, p1.x, p1.y);
		else lineHigh(p1.x, p1.y, p2.x, p2.y);
	}
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 500, 0, 500);
	glFlush();
}

void mouse(int button, int event, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && event == GLUT_DOWN)
		p1 = {.x = x, .y = 500 - y};
	else if (button == GLUT_LEFT_BUTTON && event == GLUT_UP) {
		p2 = {.x = x, .y = 500 - y};
		drawLine(p1, p2);
	}
}

void display(void) {}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Drawing");
	init();

	glutDisplayFunc(display);
	glutMouseFunc(mouse);

	glutMainLoop();

	return 0;
}