#pragma once

namespace System
{
	class RAPI raFNormalize
	{
	public:
		raFNormalize(void);
		~raFNormalize(void);

		virtual bool Create(raSmartPointer<raFSystem> System)
		{
			m_system->FSystem->createDSPByType(FMOD_DSP_TYPE_NORMALIZE, &m_effect);
			m_system = System;
			return true;
		}
		virtual void Destroy()
		{
			Stop();
			SAFE_DELETE(m_effect);
		}

		virtual bool Start()
		{
			bool active;
			if(!m_effect) return false;

			FMOD_RESULT result = m_effect->getActive(&active);

			if (!active)
			{
				RINFO("DSP::Normalize gestartet");
				result = m_system->FSystem->addDSP(((FMOD::DSP*)m_effect), 0);
			}
			return true;
		}
		virtual bool Stop()
		{
			bool active;

			if(!m_effect) return false;

			FMOD_RESULT result = m_effect->getActive(&active);

			if (active)
			{
				RINFO("DSP::Normalize gestoppt");
				result = m_effect->remove();
			}
			SAFE_DELETE(m_effect);
			return true;
		}
		PROPERTY(raSmartPointer<raFSystem>, System);
		GET(System) { return m_system; }
		SET(System) { m_system = value; }

		READONLY_PROPERTY(FMOD::DSP*, DSP);
		GET(DSP) { return m_effect; }

	private:
		FMOD::DSP						*m_effect;
		raSmartPointer<raFSystem> 		m_system;
	};
}