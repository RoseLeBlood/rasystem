#pragma once

namespace raSystem
{
	#define RAFMOD_EFFECT_ECHO				0
	#define RAFMOD_EFFECT_NORMALIZE			1
	#define RAFMOD_EFFECT_LOWPASS			3
	#define RAFMOD_EFFECT_HIGHPASS			4
	#define RAFMOD_EFFECT_FLANGE			5
	#define RAFMOD_EFFECT_DISTORTION		6
	#define RAFMOD_EFFECT_CHORUS			7
	#define RAFMOD_EFFECT_PARAMEQ			8


	class RAPI raFMODEffect : public raEffect
	{
	public:
		raFMODEffect(raFMODSystem *Sys) : raEffect(), m_System(Sys)		{}
		virtual ~raFMODEffect(void)	{ Destroy(); };

		virtual bool Create(void);
		virtual void Destroy(void) {};

		virtual bool Start(void);
		virtual bool Stop(void);
	protected:
		raFMODSystem		*m_System;
		FMOD::DSP			*m_effect;
	};

};