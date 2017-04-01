	class RAPI raGrafikDriver
	{
		friend bool operator == (const raGrafikDriver& a, const raGrafikDriver& b);
	public:
		raGrafikDriver(void);
		virtual ~raGrafikDriver(void) = 0;

		virtual bool Init(HWND wnd) = 0;
		virtual void Exit(void) = 0;
		virtual bool Resize(void) = 0;

		virtual void Clear(raColor clearColor, UINT ClearFlag, float Depth, UINT8 Stencil) = 0;
		virtual void Present() = 0;
		virtual void CreateScreenShot(const  raString SaveFile) = 0;

		HWND					GetClientWindow(void)		{ return m_clientWindow; }
		bool					IsPaused()					{ return m_standby; }
		LPCSTR					GetDriverName(void)			{ return m_DriverName; }

	protected:
		raGrafikDriver(const GUID& raguid);
	protected:

		bool					m_standby;
		bool					m_IsFullScreen;
		HWND					m_clientWindow;

		LPCSTR					m_DriverName;
		int					    m_rondomID;
	};
	inline bool operator == (const raGrafikDriver& a, const raGrafikDriver& b)						{ return ((a.m_DriverName == b.m_DriverName) && (a.m_rondomID == b.m_rondomID)); }