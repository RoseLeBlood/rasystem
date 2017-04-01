#pragma once

namespace GUI
{
	class RGAPI raOpenGl : public raElement<raOpenGl>
	{
	public:
		bool Create (System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID);

		virtual long OnMessage(Event e, EventParam ep)
		{
			return 0;
		}
	protected:
		bool SetupGLContext(bool iSetPixelFormat);
		int SetPixelFormat(HDC hdc);
	private:
		raLabel				*m_gl;
		HDC					m_hDC;
		HGLRC				m_hGLRC;
		GLuint				m_PixelFormat;
	};
}