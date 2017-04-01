#include "..\include\raMain.h"

namespace System
{
	raConfig			raConfigLoader::m_config;

	bool raConfigLoader::LoadStandardConfig(void)
	{
		m_config.Hoehe						= 800;
		m_config.weite						= 600;
		m_config.FullScreen					= false;
		m_config.vsync						= true;
		m_config.MultiSamplingQualitet		= 0;
		m_config.MultiSamplingCount			= 8;

		m_config.Soundkarte					= 0;
		m_config.Driver						= 1;
		m_config.Channels					= 2;
		m_config.SampleRate					= 48000;
		m_config.Bit						= 16;

		std::cout << "Use std Config " << std::endl;
		return true;
	}

	bool raConfigLoader::LoadConfig(IConfigFile* mconfig)
	{
		if(!mconfig)
		{
			RERROR_NULLPOINTER("mconfig");
			LoadStandardConfig();
			return false;
		}

			m_config.Hoehe						= mconfig->GetInt("grafik", "hoehe");
			m_config.weite						= mconfig->GetInt("grafik", "weite");
			m_config.FullScreen					= mconfig->GetBool("grafik", "fullscreen");
			m_config.vsync						= mconfig->GetBool("grafik", "vsync");
			m_config.MultiSamplingQualitet		= mconfig->GetInt("grafik", "multisamplingqualitet");
			m_config.MultiSamplingCount			= mconfig->GetInt("grafik", "multisamplingcount");

			m_config.Soundkarte					= mconfig->GetInt("sound", "soundkarte");
			m_config.Driver						= mconfig->GetInt("sound", "ausgabe");
			m_config.Channels					= mconfig->GetInt("sound", "channels");
			m_config.SampleRate					= mconfig->GetInt("sound", "samplerate");
			m_config.Bit						= mconfig->GetInt("sound", "bit");

			if(m_config.Hoehe == -1 ||
			   m_config.weite == -1 ||
			   m_config.MultiSamplingQualitet == -1 ||
			   m_config.MultiSamplingCount == -1 ||
			   m_config.Soundkarte == -1 ||
			   m_config.Driver == -1 ||
			   m_config.Channels == -1 ||
			   m_config.SampleRate == -1 ||
			   m_config.Bit == -1)
			{
				LoadStandardConfig();
			}

		return true;
	}
};