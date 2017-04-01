#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	LRESULT WINAPI window_subWindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
	{
		return ((raWindow*)dwRefData)->OnMessage(Event(hWnd, uMsg), EventParam(lParam, wParam));
	}
	bool raWindow::Create (DWORD dwstyle, char *pTitle, raWindowClass *WindowClass, System::raVector2 Groeﬂe,
					 unsigned long ID)
	{
		m_ID = ID;

		::SetWindowSubclass(Wnd, window_subWindowProc, ID, (LONG_PTR)this);
		return true;
	}
	raButton* raWindow::AddButton (char *pText,
		                System::raVector2 Position,
						System::raVector2 Groeﬂe,
						unsigned long ID)
	{
		raButton *pButton = NULL;
		pButton = new raButton();
		raElement<raElementNull> *pElement = NULL;

		if (!pButton->Create (pText,
						      Position,
							  Groeﬂe,
							  m_Window,
							  ID))
		{
			return NULL;
		}

		pElement = reinterpret_cast<raElement<raElementNull>*>(pButton);

		m_Elements.push_back (pElement);

		return (pButton);
	}
	raBitmapButton* raWindow::AddBitmapButton (raString Icon,
								System::raVector2 Position,
								System::raVector2 Groeﬂe,
								unsigned long ID)
	{
		raBitmapButton *pButton = NULL;
		pButton = new raBitmapButton;
		raElement<raElementNull> *pElement = NULL;

		if (!pButton->Create ("",Icon,
						      Position,
							  Groeﬂe,
							  m_Window,
							  ID))
		{
			return (NULL);
		}

		pElement = reinterpret_cast<raElement<raElementNull>*>(pButton);

		m_Elements.push_back (pElement);

		return (pButton);
	}
	raCheckbox* raWindow::AddCheckBox(char *pText, bool Checked,
							   System::raVector2 Position,
							   System::raVector2 Groeﬂe,
							   unsigned long ID)
	{
		raCheckbox *pCkeckBox = NULL;
		pCkeckBox = new raCheckbox;
		raElement<raElementNull> *pElement = NULL;

		if (!pCkeckBox->Create (pText, Position, Groeﬂe, m_Window, ID))
		{
			return (NULL);
		}
		pCkeckBox->Check(Checked);

		pElement = reinterpret_cast<raElement<raElementNull>*>(pCkeckBox);

		m_Elements.push_back (pElement);

		return (pCkeckBox);
	}
	void raWindow::AeroEnable()
	{
		DwmExtendFrameIntoClientArea(m_Window->GetWnd(), this);
	}
	raLabel* raWindow::AddLabel(char *pText,
		                System::raVector2 Position,
						System::raVector2 Groeﬂe,
						unsigned long ID)
	{
		raLabel *pLabel = NULL;
		pLabel = new raLabel;
		raElement<raElementNull> *pElement = NULL;

		if (!pLabel->Create (pText,
						      Position,
							  Groeﬂe,
							  m_Window,
							  ID))
		{
			return (NULL);
		}

		pElement = reinterpret_cast<raElement<raElementNull>*>(pLabel);

		m_Elements.push_back (pElement);

		return (pLabel);
	}
	raRadiobox* raWindow::AddRadioBox(char *pText, bool Checked,
							   System::raVector2 Position,
							   System::raVector2 Groeﬂe,
							   unsigned long ID)
	{
		raRadiobox *pRadioBox = NULL;
		pRadioBox = new raRadiobox;
		raElement<raElementNull> *pElement = NULL;

		if (!pRadioBox->Create (pText, Position, Groeﬂe, m_Window, ID))
		{
			return (NULL);
		}
		pRadioBox->Check(Checked);

		pElement = reinterpret_cast<raElement<raElementNull>*>(pRadioBox);

		m_Elements.push_back (pElement);

		return (pRadioBox);
	}
	raEdit* raWindow::AddEditBox(char *pText, bool Checked,
		                System::raVector2 Position,
						System::raVector2 Groeﬂe,
						unsigned long ID,
						bool Password,
						bool ThirdDimension )
	{
		raEdit *pEditBox = NULL;
		pEditBox = new raEdit;
		raElement<raElementNull> *pElement = NULL;

		if (!pEditBox->Create (pText, Position, Groeﬂe, m_Window, ID, Password, ThirdDimension))
		{
			return (NULL);
		}

		pElement = reinterpret_cast<raElement<raElementNull>*>(pEditBox);

		m_Elements.push_back (pElement);

		return (pEditBox);
	}

	raList* raWindow::AddListBox (System::raVector2 Position,
						System::raVector2 Groeﬂe,
						unsigned long ID,
						bool ThirdDimension,
						bool Sort)
	{
		raList *pListBox = NULL;
		pListBox = new raList;
		raElement<raElementNull> *pElement = NULL;

		if (!pListBox->Create (Position, Groeﬂe, m_Window, ID, ThirdDimension, Sort))
		{
			return (NULL);
		}

		pElement = reinterpret_cast<raElement<raElementNull>*>(pListBox);

		m_Elements.push_back (pElement);

		return (pListBox);
	}
	raSlider* raWindow::AddSlider(System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 unsigned long ID,
					 bool ThirdDimension)
	{
		raSlider *pSlider = NULL;
		pSlider = new raSlider;
		raElement<raElementNull> *pElement = NULL;

		if (!pSlider->Create (Position, Groeﬂe, m_Window, ID, ThirdDimension))
		{
			return (NULL);
		}

		pElement = reinterpret_cast<raElement<raElementNull>*>(pSlider);

		m_Elements.push_back (pElement);

		return (pSlider);
	}
	raElapsedSlider* raWindow::AddElapsedSlider(int TimePosition, int TimeLength,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 unsigned long ID)
	{
		raElapsedSlider *pSlider = NULL;
		pSlider = new raElapsedSlider;
		raElement<raElementNull> *pElement = NULL;

		if (!pSlider->Create (TimePosition, TimeLength, Position, Groeﬂe,m_Window, ID))
		{
			return (NULL);
		}

		pElement = reinterpret_cast<raElement<raElementNull>*>(pSlider);

		m_Elements.push_back (pElement);

		return (pSlider);
	}
	raRtfBox* raWindow::AddRichTextBox(char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 unsigned long ID,
					 bool ReadOnly,
					 bool MultiLine,
					 bool ThirdDimension)
	{
		raRtfBox *pRTF = NULL;
		pRTF = new raRtfBox;
		raElement<raElementNull> *pElement = NULL;

		if (!pRTF->Create (pText, Position, Groeﬂe,m_Window, ID,ReadOnly,MultiLine,ThirdDimension ))
		{
			return (NULL);
		}

		pElement = reinterpret_cast<raElement<raElementNull>*>(pRTF);

		m_Elements.push_back (pElement);

		return (pRTF);
	}
	raDrawing* raWindow::AddDrawing(unsigned long ID)
	{
		raDrawing *pdraw = NULL;
		pdraw = new raDrawing();
		raElement<raElementNull> *pElement = NULL;

		if (!pdraw->Create (m_Window, ID))
		{
			return (NULL);
		}

		pElement = reinterpret_cast<raElement<raElementNull>*>(pdraw);

		m_Elements.push_back (pElement);

		return (pdraw);
	}
	void raWindow::SetLayred()
	{
		m_Window->SetWindowLong(GWL_EXSTYLE, m_Window->GetWindowLong(GWL_EXSTYLE) | WS_EX_LAYERED);
	}
	System::raVector2 raWindow::GetWindowSize()
	{
		RECT rc;
		::GetClientRect(m_Window->GetWnd(),&rc);
		return System::raVector2(rc.right - rc.left, rc.bottom - rc.top);
	}
	BOOL raWindow::SetWindowSize(System::raVector2 ClientSize)
	{
		return (m_Window->SetClientRect(ClientSize.x, ClientSize.y) != 0 ? true : false);
	}
	raElement<raElementNull> *raWindow::GetElement (unsigned long ID)
	{
		// Alle Elemente durchgehen
		for (m_i = m_Elements.begin(); m_i != m_Elements.end(); ++m_i)
		{
			if (((*m_i)->GetID()) == ID)
			{
				return (*m_i);
			}
		}

		raElement<raElementNull> *E = NULL;
		return E;
	}
	void raWindow::Destroy ()
	{
		// Alle Elemente durchgehen
		for (m_i = m_Elements.begin(); m_i != m_Elements.end(); ++m_i)
		{
			if((*m_i)) (*m_i)->GetHandle()->Destroy();
		}
		m_Elements.clear();
	}
	long raWindow::OnMessage(Event e, EventParam ep)
	{
			return DefSubclassProc(e.shwnd, e.smsg, ep.swParam, ep.slParam);
	}
}
}