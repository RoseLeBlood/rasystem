#pragma once

namespace GUI
{
	class raWindow; // Forward Deklaraiton

	// Das BitmapButton element
	class RGAPI raBitmapButton : public raElement<raBitmapButton>
	{
	public:
		// Erstellen eines Buttons
		bool Create (raString pText, raString Icon,
					 System::raVector2 Position,
					 System::raVector2 Groeﬂe,
					 raWND* ParentWindow,
					 unsigned long ID);

		void Destroy();

		void SetText (char *pText);
		void GetText (char *pText);

		void SetBitmap(LPCWSTR Bit);
	};
}