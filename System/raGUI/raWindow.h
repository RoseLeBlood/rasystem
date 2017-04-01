#pragma once

namespace GUI
{
	class RGAPI raWindow : public MARGINS
	{
	public:
		raWindow(raInstance* Inst) 	{ if(!Inst) throw new System::raException("NULL Pointer");  m_hInstance = Inst; };

		virtual bool Create (DWORD dwExStyle, char *pTitle,
					 raWindowClass *WindowClass,
					 System::raVector2 Groeﬂe,
					 unsigned long ID) = 0;

		void AeroEnable();
		void SetLayred();

		void Destroy ();

		raButton* AddButton (char *pText,
		                System::raVector2 Position,
						System::raVector2 Groeﬂe,
						unsigned long ID);

		raBitmapButton* AddBitmapButton (raString Icon,
								System::raVector2 Position,
								System::raVector2 Groeﬂe,
								unsigned long ID);

		raLabel* AddLabel(char *pText,
		                System::raVector2 Position,
						System::raVector2 Groeﬂe,
						unsigned long ID);

		raCheckbox* AddCheckBox(char *pText, bool Checked,
		                System::raVector2 Position,
						System::raVector2 Groeﬂe,
						unsigned long ID);

		raRadiobox* AddRadioBox(char *pText, bool Checked,
		                System::raVector2 Position,
						System::raVector2 Groeﬂe,
						unsigned long ID);

		raEdit* AddEditBox(char *pText, bool Checked,
		                System::raVector2 Position,
						System::raVector2 Groeﬂe,
						unsigned long ID,
						bool Password = false,
						bool ThirdDimension = true);

		raList* AddListBox (System::raVector2 Position,
						System::raVector2 Groeﬂe,
						unsigned long ID,
						bool ThirdDimension = true,
						bool Sort = false);

		raSlider* AddSlider(System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 unsigned long ID,
					 bool ThirdDimension = true);

		raElapsedSlider* AddElapsedSlider(int TimePosition, int TimeLength,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 unsigned long ID);

		raDrawing* AddDrawing(unsigned long ID);

		raRtfBox* AddRichTextBox(char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 unsigned long ID,
					 bool ReadOnly = false,
					 bool MultiLine = true,
					 bool ThirdDimension = true);

		raWND* AddFrame(char *pText,
		                System::raVector2 Position,
						System::raVector2 Groeﬂe,
						unsigned long ID);

		raElement<raElementNull> *GetElement (unsigned long ID);

		virtual long OnMessage(Event e, EventParam ep);

	public:
		bool           Refresh()																				{ return m_Window->Refresh(); }
		bool           SetFocus()																				{ return m_Window->SetFocus(); }

		LONG_PTR       GetWindowLong(int nIndex)																{ return m_Window->GetWindowLong(nIndex); }

		bool           SetTransparenz(BYTE bRate, COLORREF crColor = 0)											{ SetLayred(); return m_Window->SetTransparenz(bRate, crColor); }
		LONG_PTR	   SetWindowLong(int nIndex, LONG_PTR dwNewLong)											{ return m_Window->SetWindowLong(nIndex, dwNewLong); }

		BOOL AnimateWindow(DWORD dwTime, DWORD dwFlags)															{ return ::AnimateWindow(m_Window->GetWnd(), dwTime, dwFlags); }
		BOOL BringWindowToTop()																					{ return ::BringWindowToTop(m_Window->GetWnd()); }
		BOOL CloseWindow()																						{ return ::CloseWindow(m_Window->GetWnd()); }
		BOOL DestroyWindow()																					{ return ::DestroyWindow(m_Window->GetWnd()); }
		System::raVector2 GetClientSize();
		BOOL SetWindowSize(System::raVector2 ClientSize);
		raWND GetParent()																						{ return ::GetParent(m_Window->GetWnd()); }
		BOOL MoveWindow(System::raVector2 Position,
						System::raVector2 Size,
						bool bRepaint)																			{ return ::MoveWindow(m_Window->GetWnd(), Position.x, Position.y,
																															Size.x, Size.y, bRepaint); }
		BOOL SetWindowText(LPCSTR Text)																			{ return ::SetWindowText(m_Window->GetWnd(), Text); }
		BOOL ShowWindow(int cmdShow)																			{ return ::ShowWindow(m_Window->GetWnd(), cmdShow); }
		BOOL ShowWindowAsync(int cmdShow)																		{ return ::ShowWindowAsync(m_Window->GetWnd(), cmdShow); }
		void SwitchToThisWindow(bool fAltTab = false)															{ return ::SwitchToThisWindow(m_Window->GetWnd(), fAltTab); }

		System::raVector2 GetWindowSize();

		READONLY_PROPERTY(HWND, Wnd);
		GET(Wnd) { return m_Window->GetWnd(); }
		READONLY_PROPERTY(HINSTANCE, hInstance);
		GET(hInstance) { return m_hInstance->Handler; }

		PROPERTY(LPCSTR, Title);
		GET(Title) { return m_Window->GetText().c_str(); }
		SET(Title) { ::SetWindowText(Wnd, value); }

		READONLY_PROPERTY(unsigned long, ID);
		GET(ID) { return m_ID; }

		void SetMenu(raMenu* Menu)							{  ::SetMenu(m_Window->GetWnd(), Menu->GetMenu()); m_hMenu = Menu; }
	protected:
		raWindowClass*										m_WindowClass;
		std::list<raElement<raElementNull>*>				m_Elements;
		std::list<raElement<raElementNull>*>::iterator		m_i;
		unsigned long										m_ID;
		raWND*												m_Window;
		raInstance*											m_hInstance;
		raMenu												*m_hMenu;
	};
}