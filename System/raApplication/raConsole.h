#pragma once
#ifndef __RA_CONSOLE__
#define __RA_CONSOLE__

namespace System
{
	enum CONSOLE_COLOR
	{
		COLOR_BLACK       = 0,
		COLOR_INTENSITY   = FOREGROUND_INTENSITY,
		COLOR_LIGHTGREY   = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE,
		COLOR_WHITE       = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		COLOR_BLUE        = FOREGROUND_BLUE,
		COLOR_GREEN       = FOREGROUND_GREEN,
		COLOR_CYAN        = FOREGROUND_GREEN | FOREGROUND_BLUE,
		COLOR_RED         = FOREGROUND_RED,
		COLOR_PURPEL      = FOREGROUND_RED   | FOREGROUND_BLUE,
		COLOR_LIGHTBLUE   = FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
		COLOR_LIGHTGREEN  = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		COLOR_LIGHTCYAN   = FOREGROUND_GREEN | FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
		COLOR_LIGHTRED    = FOREGROUND_RED   | FOREGROUND_INTENSITY,
		COLOR_LIGHTPURPEL = FOREGROUND_RED   | FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
		COLOR_ORANGE      = FOREGROUND_RED   | FOREGROUND_GREEN,
		COLOR_YELLOW      = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	};
	class raConsole
	{
	public:
		static void Write(const char string);
		static void Write(const char* string, ...);
		static void Write(raString string, ...);
		static void Write(char* string, ...);

		static void Write(int value);
		static void Write(float value);
		static void Write(double value);
		static void Write(short value);
		static void Write(long value);
		static void Write(bool value);

		static void Write(unsigned int value);
		static void Write(unsigned long value);
		static void Write(unsigned short value);
		static void Write(unsigned char value);

		static void Write(long double value);

		static void WriteLine(const char string);
		static void WriteLine(const char* string, ...);
		static void WriteLine(std::string string, ...);
		static void WriteLine(char* string, ...);

		static void WriteLine(int value);
		static void WriteLine(float value);
		static void WriteLine(double value);
		static void WriteLine(short value);
		static void WriteLine(long value);
		static void WriteLine(bool value);

		static void WriteLine(unsigned int value);
		static void WriteLine(unsigned long value);
		static void WriteLine(unsigned short value);
		static void WriteLine(unsigned char value);

		static void WriteLine(long double value);

		static raString Read();
		static raString ReadLine();
		static int ReadKey(bool sCursor = false);

		static raString ReadXy();

		static bool SetTitel(raString Title);
		static bool Show();
		static bool Close();

		static bool SetTextColor (CONSOLE_COLOR FgColor, CONSOLE_COLOR BgColor = COLOR_BLACK);
		static bool SetBackColor(CONSOLE_COLOR BgColor) { return SetTextColor(ForgroundColor, BgColor); }

		static bool gotoxy(System::raVector2 coord);
		static bool Clear();

		static bool GetCursorPosition(System::raVector2* coord);
		static bool SetSize(System::raVector2 Size);
		static bool GetSize(System::raVector2* Size);

	public:
		static void DrawBox(System::raVector2 Start,System::raVector2 Ende,raString Text);
		static void DrawClearBox(System::raVector2 Start,System::raVector2 Ende, CONSOLE_COLOR bkcol);
		static void DrawFrame(System::raVector2 Start,System::raVector2 Ende, CONSOLE_COLOR bkcol,raString Text);
		static void DrawLine(System::raVector2 Start,System::raVector2 Ende, CONSOLE_COLOR color);
		static void DrawTextLine(System::raVector2 position,raString Text);
	protected:
		static CONSOLE_COLOR ForgroundColor;
		static CONSOLE_COLOR BackgroundColor;
	private:
		static void Start();
	private:
		static int m_forgroundcolor;
		static int m_backcolor;
	};
}
#endif