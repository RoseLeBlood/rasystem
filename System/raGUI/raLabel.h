#pragma once

namespace GUI
{
	class RGAPI raLabel : public raElement<raLabel>
	{
	public:
		bool Create (char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groe�e,
					 raWND* ParentWindow,
					 unsigned long ID);

		void Destroy();

		void SetText (char *pText);
		void GetText (char *pText);
	};
}