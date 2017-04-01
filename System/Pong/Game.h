#pragma once
#include "..\raSystem\include\raMain.h"
#include "..\raApplication\raApplication.h"

using namespace System;

#define key_F1 59
#define key_UP 72
#define key_DOWN 80
#define key_SPACE 32
#define key_ESCAPE 27
#define PONG_WIDTH 78
#define PONG_HEIGHT 22
#define PONG_SCREEN_RIGHT PONG_WIDTH-3
#define PONG_SCREEN_LEFT 5
#define PONG_SCREEN_TOP 2
#define PONG_SCREEN_BOTTOM 22

struct raball
{
	int x,y,headingX,headingY;
};
struct rapad
{
	int x,y,LEFT,RIGHT;
};

class raPong
{
public:
	raPong();
	~raPong() { }

	void Run();
protected:
	void moveBall();
	void initGame();
	void Keypressed();
	void gameLogic();
	void removeBall();
	void ShowHelp();
private:
    raball ball;
    rapad  PlayersPad,computersPad;
	int keypressed,playersScore,computersScore;
	bool autoPlay;
};