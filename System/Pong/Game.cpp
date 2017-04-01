#include "Game.h"

raPong::raPong()
{
	playersScore=0;
	computersScore=0;
	ball.x=15;
	ball.y=5;
	ball.headingX=1;
	ball.headingY=1;

	PlayersPad.x=5;
	PlayersPad.y=12;
	computersPad.x=75;
	computersPad.y=12;

	autoPlay=false;

	System::raConsole::Clear();
	System::raConsole::DrawLine(VECTOR2_2F(6,0), VECTOR2_2F(74,0), COLOR_RED);
	System::raConsole::DrawLine(VECTOR2_2F(6,21), VECTOR2_2F(74,21), COLOR_RED);

    System::raConsole::SetTextColor(COLOR_WHITE);
	System::raConsole::DrawTextLine(VECTOR2_2F(62,22), "<F1> Hilfe");
}

void  raPong::Run()
{
	while (keypressed!=key_ESCAPE)
	{
		Keypressed();
		{
			gameLogic();
			moveBall();
			Sleep(50);

			removeBall();

			System::raConsole::SetTextColor(COLOR_WHITE);
			System::raConsole::gotoxy(VECTOR2_2F(7,22));
			printf("Player Punkte:     %d ", playersScore );

			System::raConsole::gotoxy(VECTOR2_2F(7,23));
			printf("Computers Punkte:  %d ", computersScore );
		}
	}
}
void raPong::moveBall()
{
	System::raConsole::SetTextColor(COLOR_LIGHTRED);
	System::raConsole::DrawTextLine(VECTOR2_2F(ball.x,ball.y), "ß");
	System::raConsole::DrawLine(VECTOR2_2F(PlayersPad.x,PlayersPad.y),VECTOR2_2F(PlayersPad.x,PlayersPad.y+3), COLOR_LIGHTGREEN);
	System::raConsole::DrawLine(VECTOR2_2F(computersPad.x,computersPad.y),VECTOR2_2F(computersPad.x,computersPad.y+3), COLOR_LIGHTGREEN);
}

void raPong::removeBall()
{
	System::raConsole::SetTextColor(COLOR_BLACK);
	System::raConsole::DrawTextLine(VECTOR2_2F(ball.x,ball.y), " ");
	System::raConsole::DrawLine(VECTOR2_2F(PlayersPad.x,PlayersPad.y),VECTOR2_2F(PlayersPad.x,PlayersPad.y+3), COLOR_BLACK);
	System::raConsole::DrawLine(VECTOR2_2F(computersPad.x,computersPad.y),VECTOR2_2F(computersPad.x,computersPad.y+3), COLOR_BLACK);
}

void raPong::gameLogic()
{
	ball.x+=ball.headingX;
	ball.y+=ball.headingY;

	if( (ball.x>PONG_SCREEN_RIGHT-1) )
	{
		ball.headingX=-ball.headingX;
		computersScore+=10;
	}
	if( (ball.y<PONG_SCREEN_TOP) || (ball.y>PONG_SCREEN_BOTTOM-3) ) ball.headingY=-ball.headingY;

    PlayersPad.LEFT=PlayersPad.y-3;
    PlayersPad.RIGHT=PlayersPad.y+5;

	if ( (ball.y>= PlayersPad.LEFT) && (ball.y<= PlayersPad.RIGHT) && (ball.x==PlayersPad.x))
	{
		ball.headingX=-ball.headingX;
		playersScore+=10;
	}
	if (ball.x>PONG_SCREEN_RIGHT-18) computersPad.y=ball.y;

	if (ball.x<PONG_SCREEN_LEFT)
	{
		ball.x=75;
		ball.y=15;
		computersScore+=10;
	}
}
void raPong::ShowHelp()
{
	System::raConsole::SetTextColor(COLOR_WHITE);
	System::raConsole::DrawFrame(VECTOR2_2F(10, 2), VECTOR2_2F(70,12), COLOR_RED, "raPONG");

	System::raConsole::DrawTextLine(VECTOR2_2F(18,3), "Controls Player 1:  ");
	System::raConsole::DrawTextLine(VECTOR2_2F(18,4), "<W>   Bewegt den Paddel nach Oben");
	System::raConsole::DrawTextLine(VECTOR2_2F(18,5), "<S>	Bewegt den Paddel nach Unten");

	System::raConsole::DrawTextLine(VECTOR2_2F(18,10), "Druecke eine Taste ...");
	System::raConsole::ReadKey();

	System::raConsole::DrawClearBox(VECTOR2_2F(10,2),VECTOR2_2F(75,21),COLOR_BLACK);
}
void raPong::Keypressed()
{
	if (_kbhit())
	{
		switch (System::raConsole::ReadKey())
		{
		    case key_F1:
				ShowHelp();
				break;
			case key_UP:
                PlayersPad.y-=3;if (PlayersPad.y<0) PlayersPad.y=0;
				break;
			case key_DOWN:
                PlayersPad.y+=3;if (PlayersPad.y>18) PlayersPad.y=18;
				break;
		}
	}
}