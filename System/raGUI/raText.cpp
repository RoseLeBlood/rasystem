#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	bool raText::Create(System::raSmartPointer<raDrawing> drw, WCHAR* FontFamily, float Size, unsigned long ID)
	{
		m_ID = ID;
		m_Parent = drw->GetParent();
		m_Type = WIN_TEXT;
		m_drw = drw;

		HRESULT hr = DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
		if(FAILED(hr))
		{
			RERROR_DX11("DWriteCreateFactory", hr);
			return false;
		}
		m_pDWriteFactory->CreateTextFormat(FontFamily,NULL,DWRITE_FONT_WEIGHT_REGULAR,DWRITE_FONT_STYLE_NORMAL,DWRITE_FONT_STRETCH_NORMAL,
			Size, L"de-de", &m_pTextFormat);

		return true;
	}

	bool raText::RenderText(WCHAR* Text, D2D1_RECT_F layoutRect)
	{
		if(!m_pBrush)
			m_drw->m_d2dRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0x0), &m_pBrush);

		m_pBrush->SetOpacity(m_opacity);
		m_pBrush->SetColor(D2D1::ColorF(m_cColor.r, m_cColor.g, m_cColor.b, m_cColor.a));

		m_drw->m_d2dRenderTarget->DrawText(
					Text,
					wcslen(Text),
					m_pTextFormat,
					layoutRect,
					m_pBrush);

		//SAFE_DELETE(m_pBrush);

		return true;
	}
	bool raText::RenderText(WCHAR* Text, System::raVector2 position, System::raVector2 Size,
						    int TextAllignment, int ascent)
	{
		D2D1_POINT_2F origin = D2D1::Point2F(0, ascent);
		D2D1_RECT_F text_rect;
		text_rect.left = position.x;
		text_rect.top = position.y;

		switch ((TA_LEFT | TA_RIGHT | TA_CENTER) & TextAllignment)
		{
		case TA_LEFT:
			break;
		case TA_RIGHT:
			text_rect.left -= Size.y;
			break;
		case TA_CENTER:
			text_rect.left -= Size.y / 2;
			break;
		}
		text_rect.right = text_rect.left + Size.x;

		switch ((TA_TOP | TA_BOTTOM | TA_BASELINE) & TextAllignment)
		{
		case TA_TOP:
			break;
		case TA_BOTTOM:
			text_rect.top -= Size.y;
			break;
		case TA_BASELINE:
			text_rect.top -= ascent;
			break;
		}
		text_rect.bottom = text_rect.top + Size.y;

		return RenderText(Text, text_rect);
	}
}
}