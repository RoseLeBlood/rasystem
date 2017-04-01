#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	raPrintDialog::raPrintDialog(void) : raElement<raPrintDialog>()
	{
		m_Type = WIN_PRINTDIALOG;

		ZeroMemory(&m_sPrint, sizeof(PRINTDLG));

		m_sPrint.lStructSize = sizeof(PRINTDLG);
		m_sPrint.Flags = PD_RETURNDC | PD_DISABLEPRINTTOFILE | PD_NOSELECTION;
	}
	HDC raPrintDialog::Show(raWND hWnd)
	{
		m_sPrint.hwndOwner = hWnd.GetWnd();
		if (PrintDlg(&m_sPrint) != 0)
			return m_sPrint.hDC;
		else
			return NULL;
	}
}
}