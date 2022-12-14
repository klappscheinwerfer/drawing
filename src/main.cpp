#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <iostream>

struct Point {
	int x, y;
};

struct Color {
	unsigned char r, g, b;
};

enum tools: unsigned char {
	Line = 2,
	Curve = 3,
	Circle = 4,
	Ellipse = 5
};

struct Tool {
	tools id = Line;
	std::vector<Point> points;
	Color color {0, 0, 0};
};

Tool myTool;

// Display
void initWindow();
void display(void);
void createMenu();
void idle();

// Controllers
void mouse(int, int, int, int);

// Draw
void drawLine(Point, Point);
void drawCurve(std::vector<Point>);
void drawCircle(Point, Point);
void drawEllipse(Point, Point);

// Menu
void mainMenu(int);
void toolMenu(int);
void colorMenu(int);

// Misc
int getDistance(Point, Point);
int fac(int n); // Factorial
float bernstein(int i, int n, float t); // Bernstein polynomial

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	initWindow();

	createMenu();

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

void initWindow() {
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Drawing");

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 500, 0, 500);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFlush();

	glColor3ub(myTool.color.r, myTool.color.g, myTool.color.b);
}

void display(void) {
	glFlush();
}

void createMenu() {
	// Tool submenu
	int toolSubmenu = glutCreateMenu(toolMenu);
	glutAddMenuEntry("Line", 2);
	glutAddMenuEntry("Curve", 3);
	glutAddMenuEntry("Circle", 4);
	glutAddMenuEntry("Ellipse", 5);

	// Color submenu
	int colorSubmenu = glutCreateMenu(colorMenu);
	glutAddMenuEntry("Black", 1);
	glutAddMenuEntry("Red", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Blue", 4);
	glutAddMenuEntry("White", 5);

	// Main menu
	glutCreateMenu(mainMenu);
	glutAddSubMenu("Tools", toolSubmenu);
	glutAddSubMenu("Colors", colorSubmenu);
	glutAddMenuEntry("Exit", 1);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
}

void idle() {
	glutPostRedisplay();
}

void mouse(int button, int event, int x, int y) {
	glutPostRedisplay();
	switch (myTool.id) {
		case Line:
			if (button == GLUT_LEFT_BUTTON && event == GLUT_DOWN) {
				myTool.points.clear();
				myTool.points.push_back({.x = x, .y = 500 - y});
			}
			else if (button == GLUT_LEFT_BUTTON && event == GLUT_UP) {
				myTool.points.push_back({.x = x, .y = 500 - y});
				drawLine(myTool.points[0], myTool.points[1]);
			}
			break;
		case Curve:
			if (button == GLUT_LEFT_BUTTON && event == GLUT_DOWN && myTool.points.size() <= 12)
				myTool.points.push_back({.x = x, .y = 500 - y});
			else if ((button == GLUT_RIGHT_BUTTON && event == GLUT_DOWN && myTool.points.size() >= 3) || myTool.points.size() == 12) {
				drawCurve(myTool.points);
				myTool.points.clear();
			}
			break;
		case Circle:
			if (button == GLUT_LEFT_BUTTON && event == GLUT_DOWN) {
				myTool.points.clear();
				myTool.points.push_back({.x = x, .y = 500 - y});
			}
			else if (button == GLUT_LEFT_BUTTON && event == GLUT_UP) {
				myTool.points.push_back({.x = x, .y = 500 - y});
				drawCircle(myTool.points[0], myTool.points[1]);
			}
			break;
		case Ellipse:
			if (button == GLUT_LEFT_BUTTON && event == GLUT_DOWN) {
				myTool.points.clear();
				myTool.points.push_back({.x = x, .y = 500 - y});
			}
			else if (button == GLUT_LEFT_BUTTON && event == GLUT_UP) {
				myTool.points.push_back({
					.x = std::abs(x - myTool.points[0].x),
					.y = std::abs(500 - y - myTool.points[0].y)});
				drawEllipse(myTool.points[0], myTool.points[1]);
			}
			break;
	}

	glFlush();
}

void drawLine(Point p1, Point p2) {
	auto lineLow = [](int x0, int y0, int x1, int y1) { 
		float dx = x1 - x0;
		float dy = y1 - y0;
		float yi = 1;
		if (dy < 0) {
			yi = -1;
			dy = -dy;
		}
		float D = 2 * dy - dx;
		float y = y0;

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
	};

	auto lineHigh = [](int x0, int y0, int x1, int y1) { 
		float dx = x1 - x0;
		float dy = y1 - y0;
		float xi = 1;
		if (dx < 0) {
			xi = -1;
			dx = -dx;
		}
		float D = 2 * dy - dx;
		float x = x0;

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

void drawCurve(std::vector<Point> points) {
	float distance = 0;
	for (int i = 1; i < points.size(); i++)
		distance += getDistance(points[i-1], points[i]);

	int n = points.size() - 1;
	float step = 1 / distance;

	glBegin(GL_POINTS);

	for (float t = 0; t < 1; t += step) {
		float x = 0, y = 0;
		for (int i = 0; i <= n; i++) {
			float b = bernstein(i, n, t);
			x += points[i].x * b;
			y += points[i].y * b;
		}
		glVertex2i((int)x, (int)y);
	}

	glEnd();
}

void drawCircle(Point p1, Point p2) {
	int x = 0;
	int y = getDistance(p1, p2);
	int p = 1 - y;

	glBegin(GL_POINTS);

	while (x < y) {
		if (p < 0) {
			x++;
			p += 2 * x + 1;
		} else {
			x++;
			y--;
			p += 2 * x - 2 * y + 1;
		}

		glVertex2i(p1.x + x, p1.y + y);
		glVertex2i(p1.x - x, p1.y + y);
		glVertex2i(p1.x + x, p1.y - y);
		glVertex2i(p1.x - x, p1.y - y);
		glVertex2i(p1.x + y, p1.y + x);
		glVertex2i(p1.x - y, p1.y + x);
		glVertex2i(p1.x + y, p1.y - x);
		glVertex2i(p1.x - y, p1.y - x);
	}

	glEnd();
}

void drawEllipse(Point c, Point r) {
	float dx, dy, d1, d2, x, y;
	x = 0;
	y = r.y;
 
	d1 = (r.y * r.y) - (r.x * r.x * r.y) + (0.25 * r.x * r.x);
	dx = 2 * r.y * r.y * x;
	dy = 2 * r.x * r.x * y;

	glBegin(GL_POINTS); 

	while (dx < dy) {
		glVertex2i(c.x + x, c.y + y);
		glVertex2i(c.x - x, c.y + y);
		glVertex2i(c.x + x, c.y - y);
		glVertex2i(c.x - x, c.y - y);
 
		if (d1 < 0) {
			x++;
			dx = dx + (2 * r.y * r.y);
			d1 = d1 + dx + (r.y * r.y);
		} else {
			x++;
			y--;
			dx = dx + (2 * r.y * r.y);
			dy = dy - (2 * r.x * r.x);
			d1 = d1 + dx - dy + (r.y * r.y);
		}
	}
 
	d2 = ((r.y * r.y) * ((x + 0.5) * (x + 0.5)))
		+ ((r.x * r.x) * ((y - 1) * (y - 1)))
		- (r.x * r.x * r.y * r.y);
 
	while (y >= 0) {
		glVertex2i(c.x + x, c.y + y);
		glVertex2i(c.x - x, c.y + y);
		glVertex2i(c.x + x, c.y - y);
		glVertex2i(c.x - x, c.y - y);
 
		if (d2 > 0) {
			y--;
			dy = dy - (2 * r.x * r.x);
			d2 = d2 + (r.x * r.x) - dy;
		} else {
			y--;
			x++;
			dx = dx + (2 * r.y * r.y);
			dy = dy - (2 * r.x * r.x);
			d2 = d2 + dx - dy + (r.x * r.x);
		}
	}

	glEnd();
}

void mainMenu(int choice) {
	switch (choice) {
		case 1: // Exit
			exit(0);
			break;
	}
}

void toolMenu(int choice) {
	switch (choice) {
		case 2 ... 5:
			myTool.id = (tools) choice;
			break;
	}
	myTool.points.clear();
}

void colorMenu(int choice) {
	switch (choice) {
		case 1: myTool.color = Color{0, 0, 0}; break;
		case 2: myTool.color = Color{255, 0, 0}; break;
		case 3: myTool.color = Color{0, 255, 0}; break;
		case 4: myTool.color = Color{0, 0, 255}; break;
		case 5: myTool.color = Color{255, 255, 255}; break;
	}
	glColor3ub(myTool.color.r, myTool.color.g, myTool.color.b);
}

int getDistance(Point p1, Point p2) {
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) * 1.0);
}

// Factorial
int fac(int n) {
	int r = 1;
	while (n) {
		r *= n;
		n--;
	}
	return r;
}

float bernstein(int i, int n, float t) {
	float r = (float)fac(n) / (fac(i) * fac(n - i));
	r *= pow(t, i);
	r *= pow(1 - t, n - i);
	return r;
}