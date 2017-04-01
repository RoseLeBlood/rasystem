#pragma once

namespace GUI
{
	class RGAPI raGameWindow : public raWindow
	{
	public:
		raGameWindow(raInstance* hInst);
		~raGameWindow(void);

		// dwExStyle, Groeﬂe werden nicht verwendet
		// Titel immer: "raSystem - " pTitle
		virtual bool Create (DWORD dwExStyle,char *pTitle,
						 raWindowClass *WindowClass,
						 System::raVector2 Groeﬂe,
						 unsigned long ID);

		System::raDirectX		GetGrafikDriver()		{ return m_dx; }

		virtual void Begin()
		{
			m_dx.Clear(COLOR_XRGB(0,0,255));
		}
		virtual void End()
		{
			m_dx.Present();
		}
	private:
		System::raDirectX				m_dx;
	};
}