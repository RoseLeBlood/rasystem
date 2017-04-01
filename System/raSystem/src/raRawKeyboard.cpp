#include "..\include\raMain.h"

namespace System
{
	raRawKeyboard::raRawKeyboard(void) : IRARawDevice()
	{
	}

	raRawKeyboard::~raRawKeyboard(void)
	{
	}
	void  raRawKeyboard::OnInput(RAWINPUT buffer)
	{
		if (buffer.header.dwType == RIM_TYPEKEYBOARD)
		{
			m_keydata = buffer.data.keyboard;
		}
	}
	RAWINPUTDEVICE raRawKeyboard::Create(HWND wnd, bool Legacy)
	{
		RAWINPUTDEVICE m_rid;

		m_rid.usUsagePage = 0x01;
		m_rid.usUsage = 0x06;
		m_rid.dwFlags = Legacy ? RIDEV_NOLEGACY : 0;   // adds HID keyboard
		m_rid.hwndTarget = 0;

		return m_rid;
	}

	bool raRawKeyboard::KeyDown(unsigned short vkKey)
	{
		return m_keydata.VKey == vkKey && !(m_keydata.Flags & RI_KEY_MAKE);
	}
	bool raRawKeyboard::KeyUp(unsigned short vkKey)
	{
		return m_keydata.VKey == vkKey && m_keydata.Flags & RI_KEY_BREAK;
	}
	bool raRawKeyboard::KeyPress(unsigned short vkKey)
	{
		return m_keydata.VKey == vkKey;
	}
}