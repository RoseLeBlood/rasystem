#include "../raSystem/include/raMain.h"
#include <dwmapi.h>
#include <Uxtheme.h>
#include <new.h>
#include <stdexcept>
#include <Shobjidl.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <d2d1.h>
#include <d2d1helper.h>
#pragma comment(lib, "d2d1.lib")

#include <commctrl.h>
#include <Commdlg.h>

#ifdef RAGUI_EXPORTS
#define RGAPI __declspec(dllexport)
#else
#define RGAPI __declspec(dllimport)

#endif
#pragma warning( disable : 4244 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4311 )

#pragma comment( lib, "Comdlg32.lib" )
#pragma comment( lib, "comctl32.lib" )
#pragma comment( lib, "Dwmapi.lib" )
#pragma comment( lib, "UxTheme.lib" )
#pragma comment( lib, "Opengl32.lib" )

#define RGDLL (GetModuleHandleA("raGui.dll"))
#define RGUIDLL RGDLL;

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include "..\raApplication\raException.h"

namespace System
{
	#include "raWND.h"
	#include "rahInstance.h"
	#include "raWindowClass.h"
	#include "raMenu.h"

	#include "raElement.h"
	#include "raButton.h"
	#include "raBitmapButton.h"
	#include "raLabel.h"
	#include "raCheckBox.h"
	#include "raRadiobox.h"
	#include "raEdit.h"
	#include "raTrayIcon.h"
	#include "raListBox.h"
	#include "raSlider.h"
	#include "raElapsedSlider.h"
	#include "raDrawing.h"
	#include "raRtfBox.h"
	#include "raRegistry.h"
	//-------------------------------------------------
	#include "raWindow.h"
	#include "raOverlappedWindow.h"
	#include "raFixedWindow.h"
	#include "raPopupWindow.h"
	#include "raOpenGl.h"
	#include "raText.h"
	#include "raGameWindow.h"
	#include "raColorDialog.h"
	#include "raPrintDialog.h"
	#include "raFileDialog.h"
	//-----------------------------
	#include "raGuiManager.h"
}