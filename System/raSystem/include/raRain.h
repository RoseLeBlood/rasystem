#pragma once

	class RAPI raRain
	{
	public:
		raRain(raSmartPointer<raDirectX> dx, raCamera *Cam, LPCSTR EffectFile = "raEffects\\ParticleEffect.fx",
				LPCSTR Texture = "raGrafik\\snowflake1.png");

		raRain(raDirectX *dx, raCamera *Cam, raTexturedMaterial *mat);
		~raRain(void);

		raParticleSystem *GetObject(void)			{ return m_pParticleSystem; }

		void OnRender(float fTime, float fRunTime);

	protected:
		void makeParticles(void);
	protected:
		raParticleSystem *m_pParticleSystem;
		raCamera		 *m_cam;
	};