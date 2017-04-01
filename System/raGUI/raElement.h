#pragma once

namespace GUI
{
	enum RAELEMENT
	{
		WIN_BUTTON, //
		WIN_LABEL, //
		WIN_EDIT, //
		WIN_CHECK, //
		WIN_COMBO,
		WIN_FRAME,
		WIN_RADIO, //
		WIN_LIST, //
		WIN_SLIDER, //
		WIN_ESLIDER, //
		WIN_GRAFIK, //
		WIN_OPENGL,  // TODO
		WIN_TEXT,	 //
		WIN_BITMAPBUTTON, //
		WIN_FILEDIALOG,  //
		WIN_PRINTDIALOG, //
		WIN_COLORDIALOG, //
		WIN_FOLDERDIALOG,
		WIN_RTF, //
		WIN_REGESTRY,
		WIN_USER
	};
	// Basis aller Element Klassen
	template<class TYPE> class raElement
	{
		friend class raElement<TYPE>;
	public:
		raElement()  {};

		bool SetFont(UINT Size, raString Text, bool Bold, bool Italic, bool Underlined, bool Crossed)
		{
			HFONT Font = NULL;
			unsigned int Weight = 0;

			if (Bold)
				Weight = 1500;

			if ((Font = CreateFont (Size, 0, 0, 0,
									 Weight, Italic,
									 Underlined, Crossed,
									 DEFAULT_CHARSET,
									 OUT_TT_PRECIS,
									 CLIP_DEFAULT_PRECIS,
									 CLEARTYPE_QUALITY,
									 FF_DONTCARE | DEFAULT_PITCH,
									 Text.c_str())) == NULL)
			{
				return false;
			}

			SendMessage (m_Element->GetWnd(), WM_SETFONT, reinterpret_cast<WPARAM>(Font), 0);

			return true;
		}
		void SetTextColor(System::raColor Col)
		{
			HDC hdc = GetDC(m_Element->GetWnd());
			::SetTextColor(hdc, RGB(Col.r, Col.g, Col.b));
		}
		bool Position(int x, int y, int nWidth, int nHeight, bool bRepaint = true)
		{
			return (::MoveWindow(m_Element->GetWnd(), x, y, nWidth, nHeight, bRepaint) != FALSE);
		}
		bool Focus()
		{
			return (::SetFocus(m_Element->GetWnd()) != NULL);
		}
		bool IsFocus()
		{
			return (m_Element->GetWnd() == ::GetFocus());
		}
		bool Enable(bool bEnabled)
		{
			return (EnableWindow(m_Element->GetWnd(), bEnabled) != FALSE);
		}
		bool Visible(bool bVisible)
		{
			if (bVisible)
				return (ShowWindow(m_Element->GetWnd(), SW_SHOW) != FALSE);
			else
				return (ShowWindow(m_Element->GetWnd(), SW_HIDE) != FALSE);
		}

		System::raRECT GetRect ()
		{
			RECT TempRect;
			GetWindowRect(m_Element->GetWnd(), &TempRect);
			return TempRect;
		}

		virtual ULONG	GetID()								{return m_ID;}
		virtual raWND*	GetHandle()							{return m_Element;}  // Nicht bei Dialog
		virtual raWND*	GetParent()							{return m_Parent;}  // Nicht bei Dialog
		virtual RAELEMENT	GetType()						{return m_Type;}

		operator raWND* ()									{return m_Element; }

	protected:
		raWND*			m_Element;
		raWND*			m_Parent;
		ULONG			m_ID;
		RAELEMENT		m_Type;
	};
	class raElementNull : public raElement<raElementNull>
	{
	};
}