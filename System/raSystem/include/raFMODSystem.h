#pragma once

namespace raSystem
{
	bool ERRCHECK(FMOD_RESULT result);

	class RAPI raFMODSystem :  public raSoundDriver
	{
	public:
		raFMODSystem(raCamera *Cam);

		virtual bool Create(raConfig* altConfig = NULL, int toUseMemory = RASYSTEMEMMORYUSE);
		virtual void Destroy();
		virtual bool Update();

		FMOD::System			*GetSystem(void)		{ return m_system; }
		FMOD_SPEAKERMODE		GetSpeadkerMode(void)	{ return m_speakermode; }
		FMOD_SOUND_FORMAT		GetFormat(void)			{ return m_format; }

	private:
		FMOD::System			*m_system;
		FMOD_SPEAKERMODE		m_speakermode;
		FMOD_SOUND_FORMAT		m_format;
	};
};