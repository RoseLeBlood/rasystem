#pragma once

namespace GUI
{
	enum WindowType
	{
		WT_FIXED,				// Nicht gr��en anderbar
		WT_SIZED,				// Normales Fenster auch Overlapped Window gennant
		WT_POPUP				// Ein Pop Up fenster also ohne Border an Position 0,0
	};

	// Der GUI Manager von hier aus k�nnen Fenster und die Elemente daruaf einfachs halber erstellt werden
	// und die Elemente auch zur�ck geben werden
	class RGAPI raGuiManager : public IRASingleton<raGuiManager>
	{
		friend class IRASingleton<raGuiManager>;
	public:
		~raGuiManager(void);

		bool Create(raInstance* Inst)					{ if(!Inst) return false; m_ihnstance = Inst; return true; }

		/* Addet ein Standart Fenster
		   @PARAM 1: Window ExStyle
		   @PARAM 2: Titel des Fensters
		   @PARAM 3: raWindowClass
		   @PARAM 4: Gr��e des Fensters
		   @PARAM 5: Type des Fensters siehe WindowType
		   @PARAM 6: ID die Fensters
		*/
		bool AddWindow (DWORD dwExStyle,char *pTitle, raWindowClass *WindowClass, System::raVector2 Size, WindowType Type, unsigned long ID);

		// F�gt ein Custom Fenster hinzu
		// eine Template funktion TWindow = die Custom Fenster klasse
		template <class TWindow> bool AddWindow(DWORD dwExStyle,char *pTitle, raWindowClass *WindowClass, System::raVector2 Size, unsigned long ID)
		{
			TWindow *window = new TWindow(m_ihnstance);
			if(!window->Create(dwExStyle, pTitle, WindowClass, Size, ID))
				return false;
			m_Windows->Add (window);
			return true;
		}
		// Zum hinzuf�gen eines Fenster was schon erstellt w�rde
		bool AddWindow(raWindow* window);

		template <class TWindow> TWindow* GetWindowAs(unsigned long WinID) { raWindow* win = GetWindow(WinID); if(!win) return NULL; else return (TWindow*)win; }

		// GUI Manager zerst�ren
		void Destroy ();
		/* Button hinzuf�gen
			@PARAM 1: DIe Fenster ID
			@PARAM 2: Der Text des Buttons
			@PARAM 3: Die Position
			@PARAM 4: Die Groe�e
			@PARAM 5: Die ID des Elementes
			*/
		raButton* AddButton (unsigned long WinID, char *pText,
		                System::raVector2 Position,
						System::raVector2 Groe�e,
						unsigned long ID)
		{
			raWindow* win = GetWindow(WinID); if(!win) NULL;
			return (win->AddButton(pText, Position, Groe�e, ID));
		}
		// Wie Button nur ein Label
		raLabel* AddLabel(unsigned long WinID, char *pText,
		                System::raVector2 Position,
						System::raVector2 Groe�e,
						unsigned long ID)
		{
			raWindow* win = GetWindow(WinID); if(!win) NULL;
			return (win->AddLabel(pText, Position, Groe�e, ID));
		}
		/* CheckBox hinzuf�gen
			@PARAM 1: DIe Fenster ID
			@PARAM 2: Der Text des Labels der Checkbox
			@PARAM 3: Wenn true dann ist Checkbox -> checked
			@PARAM 4: Die Position
			@PARAM 5: Die Groe�e
			@PARAM 6: Die ID des Elementes
			*/
		raCheckbox* AddCheckBox(unsigned long WinID, char *pText, bool Checked,
		                System::raVector2 Position,
						System::raVector2 Groe�e,
						unsigned long ID)
		{
			raWindow* win = GetWindow(WinID); if(!win) NULL;
			return (win->AddCheckBox(pText, Checked, Position, Groe�e, ID));
		}
		// Wie Checkbox nur eine RadioBox
		raRadiobox* AddRadioBox(unsigned long WinID, char *pText, bool Checked,
		                System::raVector2 Position,
						System::raVector2 Groe�e,
						unsigned long ID)
		{
			raWindow* win = GetWindow(WinID); if(!win) NULL;
			return (win->AddRadioBox(pText, Checked, Position, Groe�e, ID));
		}
		// Zum Fenster WinID eine Editbox hinzuf�gen
		raEdit* AddEditBox(unsigned long WinID, char *pText, bool Checked,
		                System::raVector2 Position,
						System::raVector2 Groe�e,
						unsigned long ID,
						bool Password = false,
						bool ThirdDimension = true)
		{
			raWindow* win = GetWindow(WinID); if(!win) NULL;
			return (win->AddEditBox(pText, Checked, Position, Groe�e, ID, Password, ThirdDimension));
		}
		// Zum Fenster WinID eine Listbox hinzuf�gen
		raList* AddListBox (unsigned long WinID, System::raVector2 Position,
						System::raVector2 Groe�e,
						unsigned long ID,
						bool ThirdDimension = true,
						bool Sort = false)
		{
			raWindow* win = GetWindow(WinID); if(!win) NULL;
			return (win->AddListBox(Position, Groe�e, ID, ThirdDimension, Sort));
		}
		// Zum Fenster WinID einen Slider hinzuf�gen
		raSlider* AddSlider(unsigned long WinID, System::raVector2 Position,
					 System::raVector2 Groe�e,
					 unsigned long ID,
					 bool ThirdDimension = true)
		{
			raWindow* win = GetWindow(WinID);  if(!win) NULL;
			return (win->AddSlider(Position, Groe�e, ID, ThirdDimension));
		}
		// Zum Fenster WinID einen ElapsedSlider hinzuf�gen ( 00:00 {Slider} 00:00 )
		raElapsedSlider* AddElapsedSlider(unsigned long WinID, int TimePosition, int TimeLength,
					 System::raVector2 Position,
					 System::raVector2 Groe�e,
					 unsigned long ID)
		{
			raWindow* win = GetWindow(WinID); if(!win) NULL;
			return (win->AddElapsedSlider(TimePosition, TimeLength, Position, Groe�e, ID));
		}
		// Zum Fenster WinID ein direct2D object hinzuf�gen
		raDrawing* AddDrawing(unsigned long WinID, unsigned long ID)
		{
			raWindow* win = GetWindow(WinID); if(!win) NULL;
			return (win->AddDrawing(ID));
		}

		raRtfBox* AddRichTextBox(unsigned long WinID, char *pText, System::raVector2 Position, System::raVector2 Groe�e, unsigned long ID,
			bool ReadOnly = false, bool MultiLine = true, bool ThirdDimension = true)
		{
			raWindow* win = GetWindow(WinID); if(!win) NULL;
			return win->AddRichTextBox(pText, Position, Groe�e, ID,ReadOnly, MultiLine, ThirdDimension);
		}
		LRESULT SendToWindow(unsigned long WinID, int msg, EventParam ep)
		{
			raWindow* win = GetWindow(WinID); if(!win) return false;
			return ::SendMessageA(win->Wnd, msg, ep.swParam, ep.slParam);
		}

		PROPERTY(raInstance*, hInstance);
		GET(hInstance) { return m_ihnstance; }
		SET(hInstance) { m_ihnstance = value; }

		PROPERTY(HINSTANCE, Handler);
		GET(Handler) { return m_ihnstance->Handler; }
		SET(Handler) { m_ihnstance->Handler = value; }
	protected:
		// Fenster Klasse aus der Liste bekommen -> such wert die ID
		raWindow *GetWindow (unsigned long ID);
	private:
		raGuiManager(void);

		System::raArray<raWindow*>	*m_Windows;
		raInstance* m_ihnstance;
	};
}