#pragma once

namespace GUI
{
#define USER_FILE_ABORT			"__"

	class RGAPI raFileDialog : public raElement<raFileDialog>
	{
	public:
		raFileDialog();
		~raFileDialog();

		raString Open(raWindow* wnd = NULL);
		raString Save(raWindow* wnd = NULL);

		void  SetFlags(DWORD dwFlags);
		void  SetInitialDir(raString lpDir);

		void  Title(raString lpTitle);
		void  Filter(LPCTSTR lpFilter);

		// Example: "All Files", "*.*"
		void  AddExtension(LPCTSTR lpExtName, LPCTSTR lpExt);

		void  SetFlagsEx(DWORD dwFlagsEx);

		DWORD GetFlags();
		DWORD GetFlagsEx();
	protected:
		OPENFILENAME m_ofnFile;
		LPTSTR       m_lpFilter;
		size_t       m_sFilterPosition;
	};
}