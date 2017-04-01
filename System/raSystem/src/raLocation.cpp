#include "..\include\raMain.h"
using namespace std;

namespace System
{
	raLocation::raLocation(void)
	{
	}

	raLocation::~raLocation(void)
	{
	}
	bool raLocation::LoadFile(raString locationName)
    {
	    FILE* fp = fopen(locationName.c_str(), "r");
	    fscanf(fp, "%255s %255s %255s %255s",
		    &modelFileName, &locatorFileName,
		    &skyBoxTextureFileName, &entryLocatorName);

	    fclose(fp);

		return true;
    }
}