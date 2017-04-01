#include "raGuiMain.h"
#include <Commctrl.h>

namespace System
{
	namespace GUI
	{
		//-------------------------------------------------------
		raGuiManager::raGuiManager(void)
		{
			m_Windows = new System::raArray<raWindow*>();
		}

		raGuiManager::~raGuiManager(void)
		{
		}
		bool raGuiManager::AddWindow (DWORD dwExStyle,char *pTitle, raWindowClass *WindowClass, System::raVector2 Size, WindowType Type, unsigned long ID)
		{
			raWindow *pWindow = NULL;

			switch(Type)
			{
			case WT_FIXED:
				pWindow = new raFixedWindow(m_ihnstance);
				break;
			case WT_SIZED:
				pWindow = new raOverlappedWindow(m_ihnstance);
				break;
			case WT_POPUP:
				pWindow = new raPopupWindow(m_ihnstance);
				break;
			}
			if(!pWindow->Create(dwExStyle, pTitle, WindowClass, Size, ID))
				return false;

			//m_Windows.push_back (pWindow);

			m_Windows->Add(pWindow);

			return true;
		}

		bool raGuiManager::AddWindow(raWindow* pWindow)
		{
			m_Windows->Add(pWindow);
			return true;
		}
		raWindow *raGuiManager::GetWindow (unsigned long ID)
		{
			for(int i=0;i <= m_Windows->GetSize() - 1; i++)
			{
				if(m_Windows->GetAt(i)->ID == ID)
					return m_Windows->GetAt(i);
			}
			return NULL;
		}

		void raGuiManager::Destroy ()
		{
		}
	}
}