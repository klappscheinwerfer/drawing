#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>

struct Point {
	GLint x;
	GLint y;
};

Point p1, p2;

void drawLine(Point p1, Point p2) {
	GLfloat dx = p2.x - p1.x;
	GLfloat dy = p2.y - p1.y;

	GLfloat x1 = p1.x;
	GLfloat y1 = p1.y;

	GLfloat step = 0;

	if (abs(dx) > abs(dy)) {
		step = abs(dx);
	} else {
		step = abs(dy);
	}

	GLfloat xInc = dx / step;
	GLfloat yInc = dy / step;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glBegin(GL_POINTS);

	for(float i = 1; i <= step; i++) {
		glVertex2i(x1, y1);
		x1 += xInc;
		y1 += yInc;
	}

	glEnd();
	glFlush();
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 500, 0, 500);
}

void mouse(int button, int event, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && event == GLUT_DOWN) {
		p1 = {.x = mx, .y = 500 - my};
		p2 = {.x = 200, .y = 200};
		drawLine(p1, p2);
	}
}

void display(void) {}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Drawing");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}