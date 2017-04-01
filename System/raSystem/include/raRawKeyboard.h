#pragma once

	class RAPI raRawKeyboard : public IRARawDevice
	{
	public:
		raRawKeyboard(void);

		~raRawKeyboard(void);

		virtual RAWINPUTDEVICE Create(HWND wnd, bool Legacy = true);

		virtual bool KeyDown(unsigned short vkKey);
		virtual bool KeyUp(unsigned short vkKey);
		virtual bool KeyPress(unsigned short vkKey);

		virtual void OnInput(RAWINPUT buffer);
	private:
		RAWKEYBOARD			m_keydata;
	};