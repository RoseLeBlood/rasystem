#include "raGuiMain.h"

namespace System
{
namespace GUI
{
	bool raDrawing::Create(raWND* ParentWindow, unsigned long ID)
	{
		m_ID = ID;
		m_Parent = ParentWindow;
		m_Type = WIN_GRAFIK;

		RECT rc;
		::GetClientRect(ParentWindow->GetWnd(), &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
		D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_d2dFactory);
		m_d2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(ParentWindow->GetWnd(), size), &m_d2dRenderTarget);

		return true;
	}
	void raDrawing::Begin(System::raColor Color)
	{
		D2D1_SIZE_F renderTargetSize = m_d2dRenderTarget->GetSize();

		m_d2dRenderTarget->BeginDraw();
		m_d2dRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_d2dRenderTarget->Clear(D2D1::ColorF(Color.r,Color.g,Color.b, Color.a));
	}
	void raDrawing::End()
	{
		m_d2dRenderTarget->EndDraw();
	}
	void raDrawing::OnSize(Event e, EventParam ep)
	{
		int width = LOWORD(ep.slParam);
		int height = HIWORD(ep.slParam);

		if (m_d2dRenderTarget)
		{
			m_d2dRenderTarget->Resize(D2D1::SizeU(width, height));
		}
	}
	bool raDrawing::DrawFilledRect(System::raRECT rc, System::raColor col)
	{
		HRESULT hr;
		D2D1_RECT_F rectangle = D2D1::RectF(rc.left, rc.top,rc.right,rc.bottom);
		D2D1::ColorF color = D2D1::ColorF(col.r,col.g,col.b, col.a);

		ID2D1SolidColorBrush *brush;
		if(FAILED(hr = m_d2dRenderTarget->CreateSolidColorBrush( color, &brush )))
		{
			RERROR_DX11("CreateSolidColorBrush", hr);
			return false;
		}

		m_d2dRenderTarget->FillRectangle(&rectangle, brush);

		SAFE_RELEASE(brush);
		return true;
	}
	bool raDrawing::DrawLine(System::raVector2 Start, System::raVector2 End, System::raColor col)
	{
		HRESULT hr;
		D2D1_POINT_2F start = D2D1::Point2F(Start.x,Start.y);
		D2D1_POINT_2F end = D2D1::Point2F(End.x,End.y);

		D2D1::ColorF color = D2D1::ColorF(col.r,col.g,col.b, col.a);

		ID2D1SolidColorBrush *brush;
		if(FAILED(hr = m_d2dRenderTarget->CreateSolidColorBrush( color, &brush )))
		{
			RERROR_DX11("CreateSolidColorBrush", hr);
			return false;
		}

		m_d2dRenderTarget->DrawLine(start,end,brush);

		SAFE_RELEASE(brush);
		return true;
	}
	bool raDrawing::DrawPoint(System::raVector2 Point, System::raColor col)
	{
		return DrawLine(VECTOR2_2F(Point.x, Point.y),
						VECTOR2_2F(Point.x+1, Point.y+1),
						col);
	}
}
}