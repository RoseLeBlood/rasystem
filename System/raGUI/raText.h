#pragma once

namespace GUI
{
	class RGAPI raText : public raElement<raText>
	{
	public:
		~raText() { }
		bool Create(System::raSmartPointer<raDrawing> drw, WCHAR* FontFamily, float Size,  unsigned long ID);

		bool RenderText(WCHAR* Text, D2D1_RECT_F layoutRect);

		bool RenderText(WCHAR* Text, System::raVector2 position, System::raVector2 Size,
			int TextAllignment = TA_CENTER, int ascent = 0);

		PROPERTY(System::raColor, Color);
		GET(Color) { return m_cColor; }
		SET(Color) { m_cColor = value; }

		PROPERTY(float, Opacity);
		GET(Opacity) { return m_opacity; }
		SET(Opacity) { m_opacity = value; }

		PROPERTY(ID2D1SolidColorBrush*, Brush);
		GET(Brush) { return m_pBrush; }
		SET(Brush) { m_pBrush = value; }

		READONLY_PROPERTY(System::raSmartPointer<raDrawing>, Drawing);
		GET(Drawing) { return m_drw; }

	private:
		System::raSmartPointer<raDrawing> m_drw;

		IDWriteFactory*			  m_pDWriteFactory;
		IDWriteTextFormat*		  m_pTextFormat;
		ID2D1SolidColorBrush*	  m_pBrush;

		System::raColor		m_cColor;
		float					m_opacity;
	};
}