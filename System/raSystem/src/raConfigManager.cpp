#include "..\include\raMain.h"

namespace raSystem
{
	raConfigManager::raConfigManager(void) : raClass("Config Manager")
	{
	}


	raConfigManager::~raConfigManager(void)
	{
	}
	bool raConfigManager::Create()
	{
		if(m_configs.SetSize(2) != S_OK)
			return false;
			
		try
		{
			m_configs.Insert(RACONFIG_TYPE_SYSTEM, new raIni(RACONFIG_PATH_SYSTEM));
		}
		catch(const char* e)
		{
			RERROR(e);
			return false;
		}
		return true;
	}
	void raConfigManager::Destroy()
	{

	}
}
