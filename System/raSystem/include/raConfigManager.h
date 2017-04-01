#pragma once

#define RACONFIG_TYPE_SYSTEM	0x00
#define RACONFIG_PATH_SYSTEM	"./raConfig/config.ini"

namespace raSystem
{
	class RAPI raConfigManager : public raClass
	{
	public:
		raConfigManager(void);
		~raConfigManager(void);

		bool Create();
		void Destroy();

		int AddConfig(IConfigFile* mconfig)
		{
			if(m_configs.Add(mconfig) == S_OK)
				return m_configs.IndexOf(mconfig);

			return -1;
		}
		IConfigFile* GetConfig(int id)
		{
			return m_configs.GetAt(id);
		}

	private:
		raArray<IConfigFile*>		m_configs;
	};
}

