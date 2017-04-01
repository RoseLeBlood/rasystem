#pragma once

namespace GUI
{
	class RGAPI raDrawing : public raElement<raDrawing>
	{
		friend class raWindow;
		friend class raText;
	public:
		~raDrawing()	{ }

		bool Create(raWND* ParentWindow, unsigned long ID);

		void Begin(System::raColor Color);

		bool DrawFilledRect(System::raRECT rc, System::raColor col);
		bool DrawLine(System::raVector2 Start, System::raVector2 End, System::raColor col);
		bool DrawPoint(System::raVector2 Point, System::raColor col);

		void End();

		void OnSize(Event e, EventParam ep);
	protected:
		ID2D1Factory*           m_d2dFactory;
		ID2D1HwndRenderTarget*  m_d2dRenderTarget;
	};
}