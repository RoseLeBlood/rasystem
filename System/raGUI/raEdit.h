#pragma once

namespace GUI
{
	class RGAPI raEdit : public raElement<raEdit>
	{
	public:

		bool Create (char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groe�e,
					 raWND* ParentWindow,
					 unsigned long ID,
					 bool Password = false,
					 bool ThirdDimension = true);

		void Destroy();

		void SetText (char *pText);
		void GetText (char *pText);
	};
}