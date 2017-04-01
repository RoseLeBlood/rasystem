#pragma once

namespace GUI
{
	class raWindow; // Forward Deklaraiton

	// Das Button element
	class RGAPI raButton : public raElement<raButton>
	{
	public:
		// Erstellen eines Buttons
		bool Create (char *pText,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID);

		void Destroy();

		void SetText (char *pText);
		void GetText (char *pText);
	};
}