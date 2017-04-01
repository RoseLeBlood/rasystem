#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	bool raOpenGl::Create (System::raVector2 Position,
							System::raVector2 Groeße,
							raWND* ParentWindow,
							unsigned long ID)
	{
		if(!m_gl->Create("", Position, Groeße, ParentWindow, ID))
			return false;

		//SetupGLContext(true);

		return true;
	}
	bool raOpenGl::SetupGLContext(bool iSetPixelFormat)
	{
		if(m_gl == NULL)
		{
			return false;
		}
		static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			24,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};
		m_hDC = ::GetDC(m_gl->GetHandle()->GetWnd());

		if (!(m_PixelFormat=ChoosePixelFormat(m_hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return false;								// Return FALSE
		}
		if(!::SetPixelFormat(m_hDC,m_PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		{
			MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return false;								// Return FALSE
		}
		if (m_hGLRC = ::wglCreateContext(m_hDC))
		{
			if(false == wglMakeCurrent(m_hDC, m_hGLRC))
			{
				RERROR("raGUI opengl \"wglMakeCurrent\"");
				return false;
			}
			else
			{
				glClearColor(0.0, 0.0, 0.0, 1.0);
			}
		}
		/*glViewport(0,0,800,600);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f,(GLfloat)800/(GLfloat)600,0.1f,100.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	*/
		return true;
	}
	int raOpenGl::SetPixelFormat(HDC hdc)
	{
		return 0;
	}
}
}