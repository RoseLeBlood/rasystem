#pragma once

	/*
	config.ini:
		[Grafik]
			hoehe=800
			weite=600
			fullscreen=0
			vsync=1
			multisamplingqualitet=0
			multisamplingcount=1
		[Sound]
			ausgabe=0;				// 0 = WASAPI; 1 = WINMM; 2 = DSOUND
			soundkarte=0;			// Soundkarte
			samplerate=44100;		// SampleRate
			bit=16;					// 16 ; 24; 32; 0 == PCM_FLOAT
			channels=2;				// 1 = Mono; 2 = Stereo; 4 = Quad; 6 = 5.1; 8 = 7.1;
			b3d=0;
			hardware=0;
	*/

	class raConfig
	{
	public:
		raConfig() 	{ }

		UINT				Hoehe, weite;
		bool				FullScreen;
		bool				vsync;
		int					MultiSamplingQualitet;
		int					MultiSamplingCount;

		int Driver;				// 0 = WASAPI; 1 = WINMM; 2 = DSOUND
		int Soundkarte;			// Soundkarte
		int SampleRate;			// SampleRate
		int	Bit;				// 16 ; 24; 32; 0 == PCM_FLOAT
		int Channels;			// 1 = Mono; 2 = Stereo; 4 = Quad; 6 = 5.1; 8 = 7.1;
		bool b3D;
		bool Hardware;
	};

	class RAPI raConfigLoader
	{
	public:
		raConfigLoader() 	{ }

		static bool LoadConfig(IConfigFile* mconfig);
		static bool LoadStandardConfig();

		static raConfig		GetConfig()			{ return m_config; }
	private:
		static raConfig			m_config;
	};