#include "raGuiMain.h"
#include <GdiPlus.h>
using namespace Gdiplus;

namespace System
{
	namespace GUI
	{
		inline std::wstring StringToWString(const std::string& s)
		{
			std::wstring temp(s.length(),L' ');
			std::copy(s.begin(), s.end(), temp.begin());
			return temp;
		}

		bool raBitmapButton::Create (raString pText, raString Bit,
						 System::raVector2 Position,
						 System::raVector2 Groeﬂe,
						 raWND* ParentWindow,
						 unsigned long ID)
		{
			m_ID = ID;
			m_Parent = ParentWindow;
			m_Type = WIN_BITMAPBUTTON;

			/*HANDLE hBitmap= LoadImage (GetModuleHandle (NULL), Bitmap,
							IMAGE_ICON,0, 0,LR_DEFAULTCOLOR | LR_LOADFROMFILE);*/

			Bitmap oBmp(StringToWString(Bit).c_str());
			HBITMAP hBmp;
			oBmp.GetHBITMAP(0, &hBmp);

			// Button erstellen
			m_Element = new raWND();
			if(!m_Element->Create(0, "BUTTON", pText.c_str(),  WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_FLAT,  Position, Groeﬂe, *m_Parent, reinterpret_cast<HMENU>(m_ID)))
				return false;

			SendMessageA (m_Element->GetWnd(), BM_SETIMAGE, (WPARAM) IMAGE_BITMAP,(LPARAM)hBmp);

			printf("raBitmapButton { Position x: %d y: %d ; Size: x: %d y: %d ; ID: %d }",
					(int)Position.x, (int)Position.y, (int)Groeﬂe.x, (int)Groeﬂe.y, ID);

			printf("erstellt\n");

			return true;
		}
		void raBitmapButton::SetBitmap(LPCWSTR Bit)
		{
			Bitmap oBmp(Bit);
			HBITMAP hBmp;
			oBmp.GetHBITMAP(0, &hBmp);
			SendMessageA (m_Element->GetWnd(), BM_SETIMAGE, (WPARAM) IMAGE_BITMAP,(LPARAM)(HANDLE) hBmp);
		}
		void raBitmapButton::Destroy()
		{
			DestroyWindow(m_Element->GetWnd());
		}

		void raBitmapButton::SetText (char *pText)
		{
			SetWindowText(m_Element->GetWnd(), pText);
		}
		void raBitmapButton::GetText (char *pText)
		{
			GetWindowText(m_Element->GetWnd(), pText, static_cast<int>(strlen (pText)));
		}
	}
}