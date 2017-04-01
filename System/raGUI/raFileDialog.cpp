#include "raGuiMain.h"

namespace System
{
using namespace GUI;

raFileDialog::raFileDialog()  : m_lpFilter(NULL)
{
	ZeroMemory(&m_ofnFile, sizeof(OPENFILENAME));
	m_ofnFile.lStructSize     = sizeof(OPENFILENAME);
	m_ofnFile.nMaxFile        = MAX_PATH;
	m_ofnFile.nMaxFileTitle   = m_ofnFile.nMaxFile;
	m_ofnFile.Flags           = OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_HIDEREADONLY;
	m_ofnFile.lpstrFile       = new char[MAX_PATH];
	m_ofnFile.lpstrFilter="All Files (*.*)\0*.*\0";

	m_sFilterPosition = 0;
}
raFileDialog::~raFileDialog()
{
	delete m_ofnFile.lpstrDefExt;
	delete [] m_ofnFile.lpstrFile;
	if (m_lpFilter != NULL)
		delete [] m_lpFilter;

	free(&m_ofnFile);
}
raString raFileDialog::Open(raWindow* wnd)
{
	m_ofnFile.hwndOwner = (wnd) ? wnd->Wnd : NULL;
	m_ofnFile.lpstrFile[0] = '\0';

	if (!GetOpenFileName(&m_ofnFile))
		return USER_FILE_ABORT;

	return m_ofnFile.lpstrFile;
}
raString raFileDialog::Save(raWindow* wnd)
{
	m_ofnFile.hwndOwner = (wnd) ? wnd->Wnd: NULL;;
	m_ofnFile.lpstrFile[0] = ('\0');

	if (!GetSaveFileName(&m_ofnFile))
		return USER_FILE_ABORT;

	return m_ofnFile.lpstrFile;
}

void  raFileDialog::SetFlags(DWORD dwFlags)
{
	m_ofnFile.Flags = dwFlags;
}
void  raFileDialog::SetInitialDir(raString lpDir)
{
	m_ofnFile.lpstrInitialDir = lpDir.c_str();
}

void  raFileDialog::Title(raString lpTitle)
{
	m_ofnFile.lpstrTitle      = lpTitle.c_str();
}
void  raFileDialog::Filter(LPCTSTR lpFilter)
{
	m_ofnFile.lpstrFilter     = lpFilter;
}

// Example: "All Files", "*.*"
void  raFileDialog::AddExtension(LPCTSTR lpExtName, LPCTSTR lpExt)
{
	if (!m_lpFilter) m_lpFilter = new CHAR[MAX_PATH];

	for (size_t i = 0; i < strlen(lpExtName) + 1; i++)
	{
		m_lpFilter[m_sFilterPosition] = lpExtName[i];
		m_sFilterPosition++;
	}

	for (size_t i = 0; i < strlen(lpExt) + 1; i++)
	{
		m_lpFilter[m_sFilterPosition] = lpExt[i];
		m_sFilterPosition++;
	}
	m_lpFilter[m_sFilterPosition] = '\0';

	Filter(m_lpFilter);
}

void  raFileDialog::SetFlagsEx(DWORD dwFlagsEx)
{
	m_ofnFile.FlagsEx = dwFlagsEx;
}

DWORD raFileDialog::GetFlags()
{
	return m_ofnFile.Flags;
}
DWORD raFileDialog::GetFlagsEx()
{
	return m_ofnFile.FlagsEx;
}
}