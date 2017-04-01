#pragma once

	class RAPI raLocation
	{
	public:
		raLocation(void);
		~raLocation(void);

		bool LoadFile(raString FileName);
	protected:
		raLocation(const GUID& raguid);
	public:
		raString modelFileName[255];
		raString locatorFileName[255];
		raString skyBoxTextureFileName[255];
		raString entryLocatorName[255];
	};