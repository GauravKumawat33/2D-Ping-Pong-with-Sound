#include <GL/glut.h>
#include <bits/stdc++.h>
#include <time.h>
#include <cmath>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <cstdlib>
#define PI 3.14159265f

float r = 0.0;
float g = 1.0;
float b = 0.0;

using namespace std;


void ChangeToRandomColor() {
	srand((unsigned)time(NULL));
	r = (float) rand() / RAND_MAX;
	g = (float) rand() / RAND_MAX;
	b = (float) rand() / RAND_MAX;
	glClearColor(r, g, b, 1.0);
}

class game {
public:
	int OrthoWid;
	int OrthoHei;
	int WinWid;
	int WinHei;
	int WinXPos;
	int WinYPos;
	float FieldSizeX;
	float FieldSizeY;
	int delay;
	float PThickness;
	float BallSize;
	float BorderT;
	float MLineT;
	int ScoreL;
	int ScoreR;
	float TextPosX;
	float TextPosY;
	float BallSpeedX;
	float PSpeedY;

	game() {
		WinWid = 700;
		WinHei = 600;
		OrthoWid = 700;
		OrthoHei = 600;
		WinXPos = 100;
		delay = 1;
		PThickness = 10;
		BallSize = 10;
		FieldSizeX = 600;
		FieldSizeY = 400;
		BorderT = 10;
		MLineT = 5;
		ScoreL = 0;
		ScoreR = 0;
		TextPosX = 0;
		TextPosY = FieldSizeY + 20;
		BallSpeedX = 4;
		PSpeedY = 3;

	}

	void start_settings();
	void win();
	void KeyControl();
	void KeyReset();
	void DrawField();
	void DrawScore();

} settings;

class ball {
public:
	float x;
	float y;
	float vx;
	float vy;
	void move();
	void reflection();
	void draw();
} ball;

class reflector {
public:
	float x, y;
	float vy;
	float size;
	bool Up, Down, hold;
	reflector() {
		vy = 0;
		y = 0;
		Up = false;
		Down = false;
		hold = false;
	}
	void draw();
	void move();
	void care();
} leftP, rightP;

void game::KeyReset() {
	leftP.vy = 0;
	rightP.vy = 0;
}

void game::KeyControl() {
	if ((leftP.Up) && (!leftP.Down))leftP.vy = PSpeedY;
	if ((!leftP.Up) && (leftP.Down))leftP.vy = -PSpeedY;
	if ((rightP.Up) && (!rightP.Down))rightP.vy = PSpeedY;
	if ((!rightP.Up) && (rightP.Down))rightP.vy = -PSpeedY;

}

void game::start_settings() {
	leftP.size = 200;
	rightP.size = 200;
	leftP.x = -510;
	rightP.x = 510;
	while (ball.vx == 0) ball.vx = (rand() % 3 - 1) * BallSpeedX;
	ball.vy = 0;
	ball.x = 0;
	ball.y = 0;
}

void game::win() {
	if ((ScoreL == 4) || (ScoreR == 4)) {
		pid_t pid = fork();
		if (pid == 0) {
			system("canberra-gtk-play -f winsound.wav");
			exit(0);
		}
		glutTimerFunc(5000, exit, 0);
		settings.delay = 10000;
	}
	if (ball.x < leftP.x + PThickness - BallSpeedX)
	{
		rightP.hold = true;
		ScoreR++;
		ChangeToRandomColor();
		pid_t pid = fork();
		if (pid == 0) {
			system("canberra-gtk-play -f earn-point.ogg");
			exit(0);
		}
	}
	if (ball.x > rightP.x - PThickness + BallSpeedX)
	{
		leftP.hold = true;
		ScoreL++;
		ChangeToRandomColor();
		pid_t pid = fork();
		if (pid == 0) {
			system("canberra-gtk-play -f earn-point.ogg");
			exit(0);
		}
	}
}


void reflector::care() {
	if (hold) {
		ball.vx = 0;
		if (x < 0)ball.x = x + 2 * settings.PThickness;
		if (x > 0)ball.x = x - 2 * settings.PThickness;
		ball.vy = vy;
		ball.y = y;
	}
}

void game::DrawField() {
	glColor3f(1, 1, 1);
	glVertex2f(-FieldSizeX - BorderT, -FieldSizeY - BorderT);
	glVertex2f(FieldSizeX + BorderT, -FieldSizeY - BorderT);
	glVertex2f(FieldSizeX + BorderT, -FieldSizeY);
	glVertex2f(-FieldSizeX - BorderT, -FieldSizeY);

	glVertex2f(-FieldSizeX - BorderT, FieldSizeY + BorderT);
	glVertex2f(FieldSizeX + BorderT, FieldSizeY + BorderT);
	glVertex2f(FieldSizeX + BorderT, FieldSizeY);
	glVertex2f(-FieldSizeX - BorderT, FieldSizeY);

	glVertex2f(-FieldSizeX - BorderT, -FieldSizeY - BorderT);
	glVertex2f(-FieldSizeX, -FieldSizeY - BorderT);
	glVertex2f(-FieldSizeX, FieldSizeY + BorderT);
	glVertex2f(-FieldSizeX - BorderT, FieldSizeY + BorderT);

	glVertex2f(FieldSizeX, -FieldSizeY - BorderT);
	glVertex2f(FieldSizeX + BorderT, -FieldSizeY - BorderT);
	glVertex2f(FieldSizeX + BorderT, FieldSizeY + BorderT);
	glVertex2f(FieldSizeX, FieldSizeY + BorderT);

	for (float i = -FieldSizeY; i <= FieldSizeY; i += 4 * MLineT) {
		glVertex2f(-MLineT, i + MLineT);
		glVertex2f(MLineT, i + MLineT);
		glVertex2f(MLineT, i - MLineT);
		glVertex2f(-MLineT, i - MLineT);
	}
}

void game::DrawScore() {
	glColor3f(0, 0, 0);
	glRasterPos2f(TextPosX - 50, TextPosY + 20);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + ScoreL);
	glRasterPos2f(TextPosX + 30, TextPosY + 20);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, '0' + ScoreR);
	glRasterPos2f(TextPosX - 50, TextPosY + 60);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'A');
	glRasterPos2f(TextPosX + 30, TextPosY + 60);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'B');

	if (ScoreL == 4) {
		glRasterPos2f(TextPosX - 250, TextPosY + 40);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'W');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'I');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'N');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'N');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'R');
	}

	if (ScoreR == 4) {
		glRasterPos2f(TextPosX + 150, TextPosY + 40);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'W');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'I');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'N');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'N');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'E');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'R');
	}
}

void reflector::move() {
	y += vy;
	if (y < -settings.FieldSizeY + size / 2) {
		y = -settings.FieldSizeY + size / 2;
		vy = 0;


	}
	if (y > settings.FieldSizeY - size / 2) {
		y = settings.FieldSizeY - size / 2;
		vy = 0;

	}
}

void reflector::draw() {
	glColor3f(0, 0, 0);
	glVertex2f(x + settings.PThickness, y + size / 2);
	glVertex2f(x + settings.PThickness, y - size / 2);
	glVertex2f(x - settings.PThickness, y - size / 2);
	glVertex2f(x - settings.PThickness, y + size / 2);
}

void ball::reflection() {
	if ((y <= -settings.FieldSizeY) || (y >= settings.FieldSizeY))vy = -vy;
	bool f1 = fabs(double(y - leftP.y)) <= leftP.size / 2 + fabs(vy);
	if ((x <= leftP.x + settings.PThickness) && f1) {
		vx = -vx;
		vy += leftP.vy;
		ChangeToRandomColor();
		pid_t pid = fork();
		if (pid == 0) {
			system("canberra-gtk-play -f touch.ogg");
			exit(0);
		}


	}
	bool f2 = fabs(double(y - rightP.y)) <= rightP.size / 2 + fabs(vy);
	if ((x >= rightP.x - settings.PThickness) && f2) {
		vx = -vx;
		vy += rightP.vy;
		ChangeToRandomColor();
		pid_t pid = fork();
		if (pid == 0) {
			system("canberra-gtk-play -f touch.ogg");
			exit(0);
		}
	}
}


void ball::draw() {
	glVertex2f(x + settings.BallSize, y + settings.BallSize);
	glVertex2f(x + settings.BallSize, y - settings.BallSize);
	glVertex2f(x - settings.BallSize, y - settings.BallSize);
	glVertex2f(x - settings.BallSize, y + settings.BallSize);

}

void ball::move() {
	x += vx;
	y += vy;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		leftP.Up = true;
		break;
	case 'a':
		leftP.Down = true;
		break;
	case 'z':
		if (leftP.hold) {
			leftP.hold = false;
			ball.vx = settings.BallSpeedX;
		}
		break;
	case 'p':
		rightP.Up = true;
		break;
	case 'l':
		rightP.Down = true;
		break;
	case 'm':
		if (rightP.hold) {
			rightP.hold = false;
			ball.vx = -settings.BallSpeedX;
		}
		break;
	}
}

void keyboardUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		leftP.Up = false;
		break;
	case 'a':
		leftP.Down = false;
		break;
	case 'p':
		rightP.Up = false;
		break;
	case 'l':
		rightP.Down = false;
		break;
	}
}

void Timer(int value)
{
	settings.win();
	settings.KeyControl();
	leftP.move();
	rightP.move();
	ball.move();
	ball.reflection();
	leftP.care();
	rightP.care();
	settings.KeyReset();
	glutPostRedisplay();
	glutTimerFunc(settings.delay, Timer, 0);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	rightP.draw();
	leftP.draw();
	ball.draw();
	settings.DrawField();
	glEnd();
	settings.DrawScore();
	glutSwapBuffers();
}

int main(int argc, char**argv)
{
	pid_t pid = fork();
	if (pid == 0) {

		system("canberra-gtk-play -f valorant.wav");

		exit(0);
	}
	else
	{
		srand(time(NULL));
		settings.start_settings();
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
		glutInitWindowSize(settings.WinWid, settings.WinHei);
		glutInitWindowPosition(settings.WinXPos, settings.WinYPos);
		glutCreateWindow("********* GSS' OPENGL PING PONG *********");
		glutDisplayFunc(draw);
		glutTimerFunc(settings.delay, Timer, 0);
		glutKeyboardFunc(keyboard);
		glutKeyboardUpFunc(keyboardUp);
		glClearColor(r, g, b, 1.0);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-settings.OrthoWid, settings.OrthoWid, -settings.OrthoHei, settings.OrthoHei);
		glutMainLoop();
		kill(pid, SIGHUP);
	}
	return 0;
}
