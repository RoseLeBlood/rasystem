#pragma once

namespace GUI
{
	class RGAPI raRadiobox : public raElement<raRadiobox>
	{
	public:
		bool Create (char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID);

		void Destroy();

		bool IsChecked ();
		void Check (bool check);
	};
}