#pragma once

namespace GUI
{
	class RGAPI raElapsedSlider : public raElement<raElapsedSlider>
	{
	public:
		bool Create (int TimePosition, int TimeLength,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID);

		void SetTimePosition (int posms);
		int GetTimePosition ()			{ return m_timespos;   }

		void SetTimeLength (int ms);
		int GetTimeLength ()			{ return m_timelengt; }

		void SetPageSize(int pagesize);
		void SetPosition(int pos);
		int GetPosition(void);

	private:
		raString MicroSecToChar(int ms);
	private:
		raSlider			m_slider;
		raWND*				m_timePos;
		raWND*				m_timelength;

		int					m_timelengt;
		int					m_timespos;
	};
}