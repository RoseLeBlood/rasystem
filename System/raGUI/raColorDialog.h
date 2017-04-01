#pragma once

namespace GUI
{
	class RGAPI raColorDialog : public raElement<raColorDialog>
	{
	public:
		raColorDialog(void);

		System::raColor Show(raWND hWnd = NULL);

		void  SetFlags(DWORD dwFlags);
		void  SetInitialColor(System::raColor mColor);
		DWORD GetFlags();

	protected:
		tagCHOOSECOLORA   m_ccColor;
		COLORREF	m_CustomColors[16];
	};
}