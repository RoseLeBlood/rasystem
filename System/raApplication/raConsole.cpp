#include <string.h>
#include "..\raSystem\include\raMain.h"

#include "raConsole.h"

using namespace System;

int raConsole::m_forgroundcolor = COLOR_WHITE;
int raConsole::m_backcolor = COLOR_BLACK << 4;

CONSOLE_COLOR raConsole::ForgroundColor = COLOR_WHITE;
CONSOLE_COLOR raConsole::BackgroundColor = COLOR_BLACK;

#ifndef raString
#define raString std::string
#endif

const UINT cp = 1250;

void raConsole::Write(const char string)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << string;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(const char* string, ...)
{
	char	acText[1024U];	// Der zu schreibende String
	va_list	VAList;			// Variable-Argumente-Liste

	// Parameterliste anfertigen und den String erstellen
	va_start(VAList, string);
	vsprintf_s(acText, string, VAList);
	va_end(VAList);

	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << acText;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(std::string string, ...)
{
	char	acText[1024U];	// Der zu schreibende String
	va_list	VAList;			// Variable-Argumente-Liste

	// Parameterliste anfertigen und den String erstellen
	va_start(VAList, string);
	vsprintf_s(acText, string.c_str(), VAList);
	va_end(VAList);

	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << acText;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(char* string, ...)
{
	char	acText[1024U];	// Der zu schreibende String
	va_list	VAList;			// Variable-Argumente-Liste

	// Parameterliste anfertigen und den String erstellen
	va_start(VAList, string);
	vsprintf_s(acText, string, VAList);
	va_end(VAList);

	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << acText;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(int value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(float value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(double value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(short value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(long value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(bool value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(unsigned int value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(unsigned long value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(unsigned short value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(unsigned char value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value;
	SetConsoleOutputCP (oldcp);
}
void raConsole::Write(long double value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value;
	SetConsoleOutputCP (oldcp);
}
//---------------------------------------------------------------------
void raConsole::WriteLine(const char string)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << string << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(const char* string, ...)
{
	char	acText[1024U];	// Der zu schreibende String
	va_list	VAList;			// Variable-Argumente-Liste

	// Parameterliste anfertigen und den String erstellen
	va_start(VAList, string);
	vsprintf_s(acText, string, VAList);
	va_end(VAList);

	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << acText << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(std::string string, ...)
{
	char	acText[1024U];	// Der zu schreibende String
	va_list	VAList;			// Variable-Argumente-Liste

	// Parameterliste anfertigen und den String erstellen
	va_start(VAList, string);
	vsprintf_s(acText, string.c_str(), VAList);
	va_end(VAList);

	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << acText << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(char* string, ...)
{
	char	acText[1024U];	// Der zu schreibende String
	va_list	VAList;			// Variable-Argumente-Liste

	// Parameterliste anfertigen und den String erstellen
	va_start(VAList, string);
	vsprintf_s(acText, string, VAList);
	va_end(VAList);

	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << acText << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(int value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(float value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(double value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(short value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(long value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(bool value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(unsigned int value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(unsigned long value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(unsigned short value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(unsigned char value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value << std::endl;
	SetConsoleOutputCP (oldcp);
}
void raConsole::WriteLine(long double value)
{
	UINT oldcp = GetConsoleOutputCP();
	SetConsoleOutputCP (cp);
	std::cout << value << std::endl;
	SetConsoleOutputCP (oldcp);
}
//---------------------------------------------------------------------------

raString raConsole::Read()
{
	int Taste = 0;

	Taste = _getch();
	Write(Taste);
}
raString raConsole::ReadLine()
{
	raString string = "";
	std::getline(std::cin, string);
	return string;
}
int raConsole::ReadKey(bool sCursor)
{
	int Taste = 0;
	Taste = _getch();
	if(Taste == 0) // Spezielle Taste wie Pfeile usw.
	{
		Taste = _getch();
	}

	if(sCursor) Write(Taste);
	return Taste;
}
bool raConsole::SetTitel(std::string Title)
{
	return (SetConsoleTitleA (Title.c_str()) == TRUE) ? true : false;
}

bool raConsole::Show()
{
	Start();
	Clear();

	return SetTextColor(COLOR_WHITE, COLOR_LIGHTBLUE);
}

bool raConsole::Close()
{
	return FreeConsole() == TRUE ? true : false;
}

bool raConsole::SetTextColor (CONSOLE_COLOR FgColor, CONSOLE_COLOR BgColor)
{
	ForgroundColor = FgColor;
	BackgroundColor = BgColor;

	m_forgroundcolor = FgColor;
	m_backcolor = BgColor << 4;

	HANDLE hconsole = GetStdHandle (STD_OUTPUT_HANDLE);
	return (SetConsoleTextAttribute (hconsole, m_forgroundcolor | m_backcolor) == TRUE);
}

bool raConsole::gotoxy(System::raVector2 coord)
{
	HANDLE hconsole = GetStdHandle (STD_OUTPUT_HANDLE);
	COORD dwCursorPosition = { coord.x, coord.y };
	return (SetConsoleCursorPosition (hconsole, dwCursorPosition) == TRUE);
}

bool raConsole::Clear()
{
	HANDLE hconsole = GetStdHandle (STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo (hconsole, &csbi))
		return false;

	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	return (FillConsoleOutputCharacter (hconsole, ' ', dwConSize, coordScreen, &cCharsWritten)              &&
			FillConsoleOutputAttribute (hconsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten) &&
			gotoxy(VECTOR2_2F(0,0)));
}

bool raConsole::GetCursorPosition(System::raVector2* coord)
{
	HANDLE hconsole = GetStdHandle (STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	if (GetConsoleScreenBufferInfo (hconsole, &coninfo))
	{
		*coord = VECTOR2_2F(coninfo.dwCursorPosition.X, coninfo.dwCursorPosition.Y);
		return true;
	}
	else
	{
		return false;
	}
}

bool raConsole::SetSize(System::raVector2 Size)
{
	HANDLE hconsole = GetStdHandle (STD_OUTPUT_HANDLE);

	COORD size = { Size.x, Size.y };

	return SetConsoleScreenBufferSize (hconsole, size) == TRUE ? true : false;
}

bool raConsole::GetSize(System::raVector2* Size)
{
	HANDLE hconsole = GetStdHandle (STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	if (GetConsoleScreenBufferInfo (hconsole, &coninfo))
	{
		*Size = VECTOR2_2F(coninfo.dwSize.X, coninfo.dwSize.Y);
		return true;
	}
	else
	{
		return false;
	}
}
void raConsole::DrawBox(System::raVector2 Start,System::raVector2 Ende,raString Text)
{
	float i,j,m;
	{
		m=(Ende.x-Start.x);
		j=m/8;
		j=j-1;
		gotoxy(VECTOR2_2F(Start.x,Start.y));
		Write("+");
		gotoxy(VECTOR2_2F(Ende.x,Start.y));
		Write("+");
		gotoxy(VECTOR2_2F(Start.x,Ende.y));
		Write("+");
		gotoxy(VECTOR2_2F(Ende.x,Ende.y));
		Write("+");

		for (i=Start.x+1;i<Ende.x;i++)
		{
			gotoxy(VECTOR2_2F(i, Start.y));
			Write("-");
			gotoxy(VECTOR2_2F(i,Ende.y));
			Write("-");
		}

		for (i=Start.y+1;i<Ende.y;i++)
		{
			gotoxy(VECTOR2_2F(Start.x,i));
			Write("|");
			gotoxy(VECTOR2_2F(Ende.x,i));
			Write("|");
		}

		gotoxy(VECTOR2_2F(Start.x+j,Start.y));
		Write(Text);
		gotoxy(VECTOR2_2F(1,24));
	}
}
void raConsole::DrawClearBox(System::raVector2 Start,System::raVector2 Ende, CONSOLE_COLOR bkcol)
{
	float x,y;
	raConsole::SetBackColor(bkcol);

	for (y=Start.y;y<Ende.y;y++)
	{
		for (x=Start.x;x<Ende.x;x++)
		{
			gotoxy(raVector2(x,y));
			System::raConsole::Write(" ");
		}
	}
}
void raConsole::DrawTextLine(System::raVector2 position,raString Text)
{
	raConsole::gotoxy(position); raConsole::Write(Text);
}
void raConsole::DrawFrame(System::raVector2 Start,System::raVector2 Ende, CONSOLE_COLOR bkcol,raString Text)
{
	DrawClearBox(Start,Ende,bkcol);
	DrawBox(Start,Ende,Text);
}
void raConsole::DrawLine(System::raVector2 Start,System::raVector2 Ende, CONSOLE_COLOR color)
{
	CONSOLE_COLOR pix = color;
	int dy = Ende.y - Start.y;
	int dx = Ende.x - Start.x;
	int stepx, stepy;

	if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
	if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
	dy <<= 1;
	dx <<= 1;

	SetTextColor(color, BackgroundColor);
	DrawTextLine(Start, "ß");
	if (dx > dy)
	{
		int fraction = dy - (dx >> 1);
		while (Start.x != Ende.x) {
			if (fraction >= 0) {
				Start.y += stepy;
				fraction -= dx;
			}
			Start.x += stepx;
			fraction += dy;
			SetTextColor(color, BackgroundColor);
			DrawTextLine(Start, "ß");
		}
	}
	else
	{
		int fraction = dx - (dy >> 1);
		while (Start.y != Ende.y)
		{
			if (fraction >= 0)
			{
				Start.x += stepx;
				fraction -= dy;
			}
			Start.y += stepy;
			fraction += dx;
			SetTextColor(color, BackgroundColor);
			DrawTextLine(Start, "ß");
		}
	}
}
void raConsole::Start()
{
	int fh;

	AllocConsole();

	//fh = _open("CON", _O_WRONLY );

	_sopen_s(&fh, "CON", _O_WRONLY,_SH_DENYRW,0);
	*stdout = *_fdopen( fh, "w");
	setvbuf(stdout, NULL, _IONBF,0);

	_sopen_s(&fh, "CON", _O_RDONLY,_SH_DENYRW,0);
	*stdin = *_fdopen( fh, "r");
	setvbuf(stdin, NULL, _IONBF, 0);

	_sopen_s(&fh, "CON", _O_WRONLY,_SH_DENYRW,0);
	*stderr = *_fdopen( fh, "w");
	setvbuf(stderr, NULL, _IONBF, 0);

	std::cout.sync_with_stdio();
	std::cin.sync_with_stdio();
	std::cerr.sync_with_stdio();
}