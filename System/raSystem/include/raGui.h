
#pragma once
namespace raSystem
{
	// Verwendete Symbole vordefinieren
	class raGUI;
	struct RAPI raGUIVertex
	{
		raVector3		vPosition;
		float			fRHW;	
		raColor			Diffuse;
		raVector2		vTexture;	
		static DWORD	dwFVF;	
	};
	enum RAPI raGUIMessageType
	{
		RA_GMT_CREATE,			// Das Element wurde erstellt.
		RA_GMT_DELETE,			// Das Element wird gel�scht.
		RA_GMT_ENTERPAGE,		// Betreten einer Seite
		RA_GMT_MOVE,			// Das Element soll bewegt werden.
		RA_GMT_RENDER,			// Das Element soll gerendert werden.
		RA_GMT_ELEMENTCLICKED,	// Ein Element wurde angeklickt.
		RA_GMT_LBUTTONDOWN,		// Linker Mausknopf gedr�ckt
		RA_GMT_LBUTTONUP,		// Linker Mausknopf losgelassen
		RA_GMT_RBUTTONDOWN,		// Rechter Mausknopf gedr�ckt
		RA_GMT_RBUTTONUP,		// Rechter Mausknopf losgelassen
		RA_GMT_KEYDOWN,			// Tastaturtaste gedr�ckt
		RA_GMT_KEYUP,			// Tastaturtaste losgelassen
		RA_GMT_KEYREPEAT,		// Wiederholter Knopfdruck (Taste bleibt gedr�ckt)
		RA_GMT_SELECTION		// Auswahl bei einer Liste wurde ge�ndert
	};

	// ******************************************************************
	// Struktur f�r eine Nachricht
	struct RAPI raGUIMessage
	{
		raGUIMessageType	Type;			// Typ der Nachricht
		int					aiInt[8];		// 8 Integer
		float				afFloat[8];		// 8 Flie�kommazahlen
		void*				apPointer[8];	// 8 Zeiger
	};

	// ******************************************************************
	// Aufz�hlung f�r Oberfl�chenelementtypen
	enum RAPI raGUIElementType
	{
		RA_GET_FRAME,		// Rahmen
		RA_GET_BUTTON,		// Knopf
		RA_GET_TEXT,		// Text
		RA_GET_IMAGE,		// Bild
		RA_GET_CHECKBOX,	// Check-Box
		RA_GET_RADIOBOX,	// Radio-Box
		RA_GET_INPUT,		// Eingabefeld
		RA_GET_LIST			// Liste
	};

	// ******************************************************************
	// Klasse f�r ein Oberfl�chenelement
	class RAPI raGUIElement
	{
		friend class raGUI;
		friend class raGUIFrame;
		friend class raGUIButton;
		friend class raGUIText;
		friend class raGUIImage;
		friend class raGUICheckBox;
		friend class raGUIRadioBox;
		friend class raGUIInput;
		friend class raGUIList;

	private:
		// Variablen
		int					m_iID;			// ID des Elements
		raGUIElementType	m_Type;			// Typ des Elements
		int					m_iPage;		// Auf welcher Seite ist das Element?
		raGUI*				m_pGUI;			// Zu welcher Oberfl�che das Element geh�rt
		raVector2			m_vPosition;	// Position der linken oberen Ecke
		raVector2			m_vSize;		// Breite und H�he
		BOOL				m_bVisible;		// Sichtbar?
		BOOL				m_bEnabled;		// Aktiviert?
		BOOL				m_bPressed;		// Wird das Element gerade gedr�ckt?

	public:
		// Methoden
		virtual int ReceiveMessage(const raGUIMessage* pMsg) = 0; // Das Element erh�lt eine Nachricht.

		// Inline-Methoden
		int					GetID()						{return m_iID;}
		raGUIElementType	GetType()					{return m_Type;}
		int					GetPage()					{return m_iPage;}
		raGUI*				GetGUI()					{return m_pGUI;}
		const raVector2&	GetPosition()				{return m_vPosition;}
		const raVector2&	GetSize()					{return m_vSize;}
		BOOL				IsVisible()					{return m_bVisible;}
		BOOL				IsEnabled()					{return m_bEnabled;}
		BOOL				IsPressed()					{return m_bPressed;}
		void				SetPage(int iPage)			{m_iPage = iPage;}
		void				SetPosition(raVector2 vPos)	{m_vPosition = vPos;}
		void				SetSize(raVector2 vSize)	{m_vSize = vSize;}
		void				Show(BOOL bShow)			{m_bVisible = bShow;}
		void				Enable(BOOL bEnable)		{m_bEnabled = bEnable;}
	};

	// ******************************************************************
	// Klasse f�r einen Rahmen
	class RAPI raGUIFrame : public raGUIElement
	{
		friend class raGUI;

	public:
		// Methoden
		int ReceiveMessage(const raGUIMessage* pMsg); // Das Element erh�lt eine Nachricht.
	};

	// ******************************************************************
	// Klasse f�r einen Knopf
	class RAPI raGUIButton : public raGUIElement
	{
		friend class raGUI;

	private:
		// Variablen
		char	m_acText[256];		// Text
		BOOL	m_bIsListScrollBtn;	// Ist es ein Knopf einer Liste (zum Scrollen)?
		BOOL	m_bIsScrollUpBtn;	// Ist es ein Knopf zum Hochscrollen?
		int		m_iList;			// ID der zugeh�rigen Liste

	public:
		// Methoden
		int ReceiveMessage(const raGUIMessage* pMsg); // Das Element erh�lt eine Nachricht.

		// Inline-Methoden
		char*	GetText()				{return m_acText;}
		void	SetText(char* pcText)	{strcpy(m_acText, pcText);}
	};

	// ******************************************************************
	// Klasse f�r einen Text
	class RAPI raGUIText : public raGUIElement
	{
		friend class raGUI;

	private:
		// Variablen
		char		m_acText[4096];	// Text
		raColor		m_Color;		// Farbe
		raVector2	m_vTextSize;	// Gr��e
		DWORD		m_dwTextFlags;	// Flags f�r die Textdarstellung

	public:
		// Methoden
		int ReceiveMessage(const raGUIMessage* pMsg); // Das Element erh�lt eine Nachricht.

		// Inline-Methoden
		char*				GetText()						{return m_acText;}
		const raColor&		GetColor()						{return m_Color;}
		const raVector2&	GetTextSize()					{return m_vTextSize;}
		DWORD				GetTextFlags()					{return m_dwTextFlags;}
		void				SetText(char* pcText)			{strcpy(m_acText, pcText);}
		void				SetColor(const raColor& c)		{m_Color = c;}
		void				SetTextSize(const raVector2& s)	{m_vTextSize = s;}
		void				SetTextFlags(DWORD f)			{m_dwTextFlags = f;}
	};

	// ******************************************************************
	// Klasse f�r ein Bild
	class RAPI raGUIImage : public raGUIElement
	{
		friend class raGUI;

	private:
		// Variablen
		ID3D11Texture2D*	m_pTexture;			// Die Textur
		raVector2			m_vTopLeftTex;		// Linke obere Texturkoordinaten
		raVector2			m_vBottomRightTex;	// Rechte untere Texturkoordinaten
		raColor				m_Color;			// Farbe

	public:
		// Methoden
		int ReceiveMessage(const raGUIMessage* pMsg); // Das Element erh�lt eine Nachricht.

		// Inline-Methoden
		ID3D11Texture2D*	GetTexture()					{return m_pTexture;}
		raVector2			GetTopLeftTex()					{return m_vTopLeftTex;}
		raVector2			GetBottomRightTex()				{return m_vBottomRightTex;}
		raColor				GetColor()						{return m_Color;}
		void				SetTexture(ID3D11Texture2D* t)	{m_pTexture = t;}
		void				SetTopLeftTex(raVector2 v)		{m_vTopLeftTex = v;}
		void				SetBottomRightTex(raVector2 v)	{m_vBottomRightTex = v;}
		void				SetColor(raColor c)				{m_Color = c;}
	};

	// ******************************************************************
	// Klasse f�r eine Check-Box
	class RAPI raGUICheckBox : public raGUIElement
	{
		friend class raGUI;

	private:
		// Variablen
		char m_acText[256];	// Text
		BOOL m_bChecked;	// Angekreuzt?

	public:
		// Methoden
		int ReceiveMessage(const raGUIMessage* pMsg); // Das Element erh�lt eine Nachricht.

		// Inline-Methoden
		char*	GetText()				{return m_acText;}
		BOOL	IsChecked()				{return m_bChecked;}
		void	SetText(char* pcText)	{strcpy(m_acText, pcText);}
		void	Check(BOOL bCheck)		{m_bChecked = bCheck;}
	};

	// ******************************************************************
	// Klasse f�r eine Radio-Box
	class RAPI raGUIRadioBox : public raGUIElement
	{
		friend class raGUI;

	private:
		// Variablen
		char	m_acText[256];	// Text
		int		m_iGroup;		// Gruppen-ID
		BOOL	m_bChecked;		// Angekreuzt?

	public:
		// Methoden
		int ReceiveMessage(const raGUIMessage* pMsg);	// Das Element erh�lt eine Nachricht.
		int Check(BOOL bCheck);						// Status �ndern

		// Inline-Methoden
		char*	GetText()				{return m_acText;}
		BOOL	IsChecked()				{return m_bChecked;}
		int		GetGroup()				{return m_iGroup;}
		void	SetText(char* pcText)	{strcpy(m_acText, pcText);}
		void	SetGroup(int iGroup)	{m_iGroup = iGroup; Check(m_bChecked);}
	};

	// ******************************************************************
	// Klasse f�r ein Eingabefeld
	class RAPI raGUIInput : public raGUIElement
	{
		friend class raGUI;

	private:
		// Variablen
		char	m_acText[4096];	// Text
		BOOL	m_bPassword;	// Passwortfeld?
		char	m_PasswordChar;	// Zeichen f�r Passwort
		int		m_iCursor;		// Wo der Cursor gerade ist
		int		m_iScrolling;	// Ab wo der Text angezeigt wird

	public:
		// Methoden
		int ReceiveMessage(const raGUIMessage* pMsg); // Das Element erh�lt eine Nachricht.

		// Inline-Methoden
		char*	GetText()				{return m_acText;}
		BOOL	IsPassword()			{return m_bPassword;}
		char	GetPasswordChar()		{return m_PasswordChar;}
		int		GetCursor()				{return m_iCursor;}
		void	SetText(char* pcText)	{strcpy(m_acText, pcText); m_iCursor = strlen(m_acText);}
		void	SetPassword(BOOL b)		{m_bPassword = b;}
		void	SetPasswordChar(char c)	{m_PasswordChar = c;}
		void	SetCursor(int iCursor)	{m_iCursor = iCursor;}
	};

	// ******************************************************************
	// Struktur f�r einen Listeneintrag
	struct RAPI raGUIListEntry
	{
		int		iID;			// ID dieses Eintrags
		char	acText[256];	// Text
		void*	pData;			// Daten
	};

	// ******************************************************************
	// Klasse f�r eine Liste
	class RAPI raGUIList : public raGUIElement
	{
		friend class raGUI;

	private:
		// Variablen
		raList<raGUIListEntry>*	m_pList;		// Die Liste der Eintr�ge
		int						m_iCursor;		// ID des angew�hlten Listeneintrags
		int						m_iScrolling;	// ID des ersten sichtbaren Listeneintrags
		int						m_iNextEntryID;	// ID des n�chsten Eintrags, der erstellt wird
		int						m_iScrollUpBtn;	// ID des Knopfes zum Hochscrollen
		int						m_iScrollDnBtn;	// ID des Knopfes zum Runterscrollen
		float					m_fLineHeight;	// Zeilenh�he
		int						m_iOldCursor;	// Alter Cursor

	public:
		// Methoden
		int						ReceiveMessage(const raGUIMessage* pMsg);	// Das Element erh�lt eine Nachricht.
		raGUIListEntry*					GetEntry(int iID);							// Liefert einen Listeneintrag
		raListEntry<raGUIListEntry>*	GetEntryListEntry(int iID);					// Liefert einen Listeneintrag
		int								AddEntry(char* pcText, void* pData);		// F�gt einen Eintrag hinzu
		int						DeleteEntry(int iID);						// L�scht einen Eintrag
		int								GetEntryOrder(int iID);						// Liefert die Ordnung eines Eintrags
		raGUIListEntry*					GetEntryByOrder(int iOrder);				// Liefert den Eintrag mit einer bestimmten Ordnung
		int								GetPrevEntry(int iID);						// Liefert vorherigen Eintrag
		int								GetNextEntry(int iID);						// Liefert n�chsten Eintrag
		int						Correct();									// Korrigiert eventuelle Fehler

		// Inline-Methoden
		int		Clear()					{return m_pList->Clear();}
		int				GetCursor()				{return m_iCursor;}
		float			GetLineHeight()			{return m_fLineHeight;}
		int				GetNumEntries()			{return m_pList->GetNumEntries();}
		raGUIListEntry*	GetSelectedEntry()		{return GetEntry(m_iCursor);}
		void			SetCursor(int i)		{m_iCursor = i;}
		void			SetLineHeight(float f)	{m_fLineHeight = f;}
	};

	// ******************************************************************
	// Struktur f�r einen Skin
	struct RAPI raGUISkin
	{
		ID3D11Texture2D*	pSkinTexture;		// Textur f�r den Skin
		ID3DX11Effect*		pEffect;			// Effekt f�r die Oberfl�chenelemente
		raVector2			vCursorSize;		// Gr��e des Cursors
		raColor				CursorColor1;		// Farbe 1 des Cursors
		raColor				CursorColor2;		// Farbe 2 des Cursors
		raVector2			vCursorHotSpot;		// Hot-Spot des Cursors
		float				fCursorSpeed;		// Geschwindigkeit des Cursors
		raColor				EnabledColor;		// Farbe f�r aktivierte Elemente
		raColor				DisabledColor;		// Farbe f�r deaktivierte Elemente
		raColor				HighlightColor;		// Farbe f�r hervorgehobene Elemente
		raVector2			vTextSize;			// Gr��e der Texte
		raColor				EnabledTextColor;	// Farbe f�r Texte aktivierter Elemente
		raColor				DisabledTextColor;	// Farbe f�r Texte deaktivierter Elemente
		raColor				HighlightTextColor;	// Farbe f�r Texte hervorgehobener Elemente
	};

	// ******************************************************************
	// Klasse f�r die Benutzeroberfl�che
	class RAPI raGUI
	{
		friend class raGUIElement;
		friend class raGUIFrame;
		friend class raGUIText;
		friend class raGUIImage;
		friend class raGUIButton;
		friend class raGUICheckBox;
		friend class raGUIRadioBox;
		friend class raGUIInput;
		friend class raGUIList;

	public:
		// Konstruktor und Destruktor
		raGUI(raDirectX *dx);
		~raGUI() {Exit();}

		// Methoden
		int					Init(raGUISkin* pSkin, void* pMsgProc, raVector2 vSize, raVector2 vScaling = raVector2(1.0f));	// Initialisierung
		int					Exit();
		int					Move(const float fTime, BOOL* pbButtons, float* pfButtons);										// Bewegen
		int					Render(const float fTime);																		// Rendern
		int					RenderCursor(const float fTime);																// Mauszeiger rendern
		int					PostMessage(int iID, raGUIMessage* pMsg);														// Sendet eine Nachricht an ein Element
		int					BroadcastMessage(raGUIMessage* pMsg, int iPage = -1);											// Schickt eine Nachricht an alle Elemente
		raListEntry<raGUIElement*>*	GetElementListEntry(int iID);																	// Liefert den Listeneintrag eines Elements
		raGUIElement*				GetElement(int iID);																			// Liefert Zeiger auf ein Element
		int					DeleteElement(int iID);																			// L�scht ein Element
		int					SetCurrentPage(int iPage);																		// Setzt die aktive Seite

		// F�gt ein Rechteck zu den Vertizes hinzu
		int AddRect(const raVector2& vTopLeft,
						 const raVector2& vSize,
						 const raColor& Color,
						 const raVector2& vTopLeftTex,
						 const raVector2& vSizeOnTex,
						 raGUIVertex* pOut = NULL);

		// F�gt mehrere Rechtecke in einer Reihe hinzu
		int AddRects1D(const raVector2& vTopLeft,
							const raVector2& vSize,
							float fNumRects,
							const raVector2& vTranslation,
							const raColor& Color,
							const raVector2& vTopLeftTex,
							const raVector2& vSizeOnTex);

		// F�gt mehrere Rechtecke zweidimensional hinzu
		int AddRects2D(raVector2 vTopLeft,
							const raVector2& vSize,
							const float fNumColumns,
							float fNumRows,
							const raColor& Color,
							const raVector2& vTopLeftTex,
							const raVector2& vSizeOnTex);

		// F�gt einen ganzen Rahmen hinzu
		int AddFrame(const raVector2& vTopLeft,
						  const raVector2& vSize,
						  const raColor& Color,
						  const raVector2& vTopLeftOnTex);

		// Erstellt ein neues Element
		raGUIElement* CreateElement(int iID,
									raGUIElementType Type,
									int iPage,
									raVector2 vPosition,
									raVector2 vSize);

		// Erstellt einen neuen Rahmen
		int CreateFrame(int iID,
							 int iPage,
							 raVector2 vPosition,
							 raVector2 vSize);

		// Erstellt einen neuen Knopf
		int CreateButton(int iID,
							  int iPage,
							  raVector2 vPosition,
							  raVector2 vSize,
							  char* pcText);

		// Erstellt ein neues Bild
		int CreateImage(int iID,
							 int iPage,
							 raVector2 vPosition,
							 raVector2 vSize,
							 ID3D11Texture2D* pTexture,
							 raVector2 vTopLeftTex = raVector2(0.0f, 0.0f),
							 raVector2 vBottomRightTex = raVector2(1.0f, 1.0f),
							 raColor Color = raColor(1.0f));

		// Erstellt eine neue Check-Box
		int CreateCheckBox(int iID,
								int iPage,
								raVector2 vPosition,
								raVector2 vSize,
								char* pcText,
								BOOL bChecked = FALSE);

		// Erstellt eine neue Radio-Box
		int CreateRadioBox(int iID,
								int iPage,
								raVector2 vPosition,
								raVector2 vSize,
								char* pcText,
								int iGroup = 0,
								BOOL bChecked = FALSE);

		// Erstellt ein neues Eingabefeld
		int CreateInput(int iID,
							 int iPage,
							 raVector2 vPosition,
 							 raVector2 vSize,
							 char* pcText = "",
							 BOOL bPassword = FALSE,
							 char PasswordChar = '*');

		// Erstellt eine neue Liste
		int CreateList(int iID,
							int iPage,
							raVector2 vPosition,
							raVector2 vSize,
							float fLineHeight);

		// Inline-Methoden
		BOOL		WasButtonPressed(int b)		{return !m_pbOldButtons[b] && m_pbButtons[b];}
		BOOL		WasButtonReleased(int b)	{return m_pbOldButtons[b] && !m_pbButtons[b];}
		raGUISkin	GetSkin()					{return m_Skin;}
		ID3DX11Effect*	GetImageEffect()			{return m_pImageEffect;}
		int			GetCurrentPage()			{return m_iCurrentPage;}
		raVector2	GetCursorPos()				{return m_vCursorPos;}
		void*		GetMessageProc()			{return m_pMsgProc;}
		void		SetSkin(raGUISkin* pSkin)	{m_Skin = *pSkin;}
		void		SetCursorPos(raVector2 v)	{m_vCursorPos = v;}
		void		SetMessageProc(void *pProc)	{m_pMsgProc = pProc;}
	private:
		// Variablen
		raList<raGUIElement*>*	m_pElements;		// Liste der Elemente
		int						m_iFocus;			// Das Element, das den Fokus hat (ID)
		ID3D11Buffer*			m_pVertexBuffer;	// Der Vertex-Buffer
		BOOL*					m_pbButtons;		// Zustand der analogen Kn�pfe (digital)
		BOOL*					m_pbOldButtons;		// Ehemaliger Zustand der analogen Kn�pfe (digital)
		float*					m_pfButtons;		// Zustand der analogen Kn�pfe
		raGUISkin				m_Skin;				// Skin f�r die Benutzeroberfl�che
		ID3DX11Effect*			m_pImageEffect;		// Effekt f�r Bilder
		int						m_iCurrentPage;		// Aktive Seite
		raVector2				m_vCursorPos;		// Cursorposition
		raVector2				m_vSize;			// Gr��e
		raVector2				m_vScaling;			// Skalierung
		void*					m_pMsgProc;			// Funktion f�r Kommandonachrichten
		int						m_aiKeyRepeat[108];	// Array f�r wiederholten Tastendruck
		DWORD					m_dwOldTime;		// Alte Zeit
		UINT					m_nVertices;
		UINT					m_nIndices;
		raDirectX				*m_dx;
	};
}
// ******************************************************************