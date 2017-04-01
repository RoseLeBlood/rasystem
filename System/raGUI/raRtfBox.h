#pragma once

namespace GUI
{
	class raRtfBox : public raElement<raRtfBox>
	{
	public:
		bool Create (char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID,
					 bool ReadOnly = false,
					 bool MultiLine = true,
					 bool ThirdDimension = true);

		void Destroy();
	};
}