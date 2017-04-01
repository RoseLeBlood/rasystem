#pragma once

	class RAPI IRARawDevice
	{
	protected:
		IRARawDevice(void);

		~IRARawDevice(void);
	public:
		virtual RAWINPUTDEVICE Create(HWND wnd, bool Legacy = true) = 0;
		virtual void OnInput(RAWINPUT buffer) = 0;

		virtual bool KeyDown(unsigned short vkKey) = 0;
		virtual bool KeyUp(unsigned short vkKey) = 0;
		virtual bool KeyPress(unsigned short vkKey) = 0;
	};