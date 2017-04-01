#pragma once

namespace GUI
{
	class RGAPI raEdit : public raElement<raEdit>
	{
	public:

		bool Create (char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID,
					 bool Password = false,
					 bool ThirdDimension = true);

		void Destroy();

		void SetText (char *pText);
		void GetText (char *pText);
	};
}