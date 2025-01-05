#ifndef WINDOWSTATE_H
#define WINDOWSTATE_H
struct WindowState
{
	float DELTA_TIME = 0.0f;
	float LAST_FRAME = 0.0f;
	bool FIRST_MOUSE = true;
	int WIDTH = 1600;
	int HEIGHT = 1200;
	float LAST_X = 400;
	float LAST_Y = 300;
};
#endif