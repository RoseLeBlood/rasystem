#pragma once

namespace GUI
{
	class RGAPI raCheckbox : public raElement<raCheckbox>
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