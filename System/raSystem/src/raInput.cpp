#include "..\include\raMain.h"

namespace System
{
	raInput::raInput(void)
	{
	}

	raInput::~raInput(void)
	{
	}
	long raInput::OnInput(HWND wnd, WPARAM wParam, LPARAM lParam)
	{
		  UINT dwSize;

		  GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		  LPBYTE lpb = new BYTE[dwSize];

		  if (lpb == NULL)
		  {
			return 0;
		  }

		  int readSize = GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER) ) ;

		  if( readSize != dwSize )
			RERROR( "GetRawInputData" ) ;

		  RAWINPUT* raw = (RAWINPUT*)lpb;

		  for(int i=0; i<= m_devices.GetSize()-1;i++)
		  {
			  m_devices[i]->OnInput(*raw);
		  }
		  /*if (raw->header.dwType == RIM_TYPEKEYBOARD)
		  {
			if( raw->data.keyboard.VKey == VK_SPACE )
			{
			  puts( "You are pressing space" ) ;
			}
		  }
		  else if (raw->header.dwType == RIM_TYPEMOUSE)
		  {
			int dx = raw->data.mouse.lLastX ;
			int dy = raw->data.mouse.lLastY ;

			printf( "%d %d\n", dx, dy ) ;
		  } */

		  delete[] lpb;
		  return 0;
	}
	long raInput::OnInputDeviceChange(HWND wnd, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
	bool raInput::AddNewDevice(IRARawDevice *device)
	{
		if(m_devices.Add(device) == S_OK)
		{
			RINFO("Neues RawDevice Hinzugefügt");
			return true;
		}
		return false;
	}
	bool raInput::Create(HWND hWnd, bool legacy)
	{
		RAWINPUTDEVICE *dev = new RAWINPUTDEVICE[m_devices.GetSize()]();

		for(int i = 0; i<=m_devices.GetSize()-1;i++)
		{
			dev[i] = m_devices[i]->Create(hWnd, legacy);

			RINFO("rawDevice Erstellt");
		}

		if(RegisterRawInputDevices(dev, m_devices.GetSize(), sizeof(dev[0])))
		{
			RINFO("rawDevices dem Fenster hinzugefügt");
			return true;
		}
		else
		{
			RERROR("RegisterRawInputDevices");
			return false;
		}
	}
}