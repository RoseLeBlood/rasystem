#include "raGuiMain.h"

namespace System
{
using namespace GUI;

raColorDialog::raColorDialog(void) : raElement<raColorDialog>()
{
	m_Type = WIN_COLORDIALOG;
	ZeroMemory(&m_ccColor, sizeof(CHOOSECOLOR));

	m_ccColor.lStructSize  = sizeof(CHOOSECOLOR);
	m_ccColor.Flags        = CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT | CC_SHOWHELP;
	m_ccColor.lpCustColors = m_CustomColors;
	m_ccColor.rgbResult    = RGB(0, 0, 255);

	for (int i = 0; i < 16; i++)
		m_CustomColors[i] = RGB(255, 255, 255);
}
System::raColor raColorDialog::Show(raWND hWnd)
{
	m_Parent = &hWnd;

	m_ccColor.hwndOwner = hWnd.GetWnd();
	if (ChooseColorA(&m_ccColor) != 0)
		return COLOR_XRGB(GetRValue(m_ccColor.rgbResult),
						  GetGValue(m_ccColor.rgbResult),
						  GetBValue(m_ccColor.rgbResult));
	else
		return COLOR_XRGB(0,0,0);
}

void  raColorDialog::SetFlags(DWORD dwFlags)
{
	m_ccColor.Flags = dwFlags;
}
void  raColorDialog::SetInitialColor(System::raColor mColor)
{
	m_ccColor.rgbResult = RGB(mColor.r,mColor.g,mColor.b);
}
DWORD raColorDialog::GetFlags()
{
	return m_ccColor.Flags;
}
}