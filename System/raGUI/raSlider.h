#pragma once

namespace GUI
{
	class RGAPI raSlider : public raElement<raSlider>
	{
	public:
		raSlider(void);
		~raSlider(void);

		bool Create (System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID,
					 bool ThirdDimension = true);

		void Destroy();

		/*
		TBM_SETRANGE
		TBM_SETPAGESIZE
		TBM_SETSEL
		TBM_SETPOS */

		void SetRange(int iMin, int iMax);
		void SetPageSize(int pagesize);
		void SetPosition(int pos);

		int GetPosition();
	};
}