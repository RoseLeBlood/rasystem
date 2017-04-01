#pragma once

	class RAPI raInput
	{
	public:
		raInput(void);

		~raInput(void);

		bool AddNewDevice(IRARawDevice *device);
		bool Create(HWND hWnd, bool legacy = true);

		virtual long OnInput(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long OnInputDeviceChange(HWND wnd, WPARAM wParam, LPARAM lParam);

	protected:
		raArray<IRARawDevice*>			m_devices;
	};