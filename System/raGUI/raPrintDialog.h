#pragma once

namespace GUI
{
	class RGAPI raPrintDialog : public raElement<raPrintDialog>
	{
	public:
		raPrintDialog(void);

		HDC Show(raWND hWnd = NULL);

		void SetFlags(DWORD dwFlags) { m_sPrint.Flags = dwFlags; }
		DWORD GetFlags() { return m_sPrint.Flags; }

	protected:
		PRINTDLG m_sPrint;
	};
}