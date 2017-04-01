#include "..\include\raMain.h"

namespace System
{
	raRain::raRain(raSmartPointer<raDirectX> dx, raCamera *Cam, LPCSTR EffectFile, LPCSTR Texture) : m_cam(Cam)
	{
		raRain(dx.get(), Cam, new System::raTexturedMaterial(dx.get(),  EffectFile, "RenderParticle", Texture));
	}
	raRain::raRain(raDirectX *dx, raCamera *Cam, raTexturedMaterial *mat) : m_cam(Cam)
	{
		m_pParticleSystem = new raParticleSystem(dx, mat, false, 0.05f, -0.0981f, 15.0f);
	}
	void raRain::OnRender(float fTime, float fRunTime)
	{
		if(m_pParticleSystem)
		{
			static float t = 0;
			t += fTime;
			if( t > 1.0f)
			{
				makeParticles();
				t = 0.0f;
			}
		}
	}
	void raRain::makeParticles(void)
	{
		const raVector3* p = m_cam->GetEyePt();
		for(int i = -50; i <= 50; i+= 5)
		{
			for(int j = -50; j <= 50; j+= 5)
			{
				m_pParticleSystem->AddParticle(
					(float)p->x + i + (((float)rand() / (float)RAND_MAX) - 0.5f) * 5.0f,
					(float)p->y + 5 + (((float)rand() / (float)RAND_MAX) - 0.5f) * 5.0f,
					(float)p->z + j + (((float)rand() / (float)RAND_MAX) - 0.5f) * 5.0f,
					(((float)rand() / (float)RAND_MAX) - 0.5f) * 0.1f,
					(((float)rand() / (float)RAND_MAX) - 0.5f) * 0.1f,
					(((float)rand() / (float)RAND_MAX) - 0.5f) * 0.1f);
			}
		}
	}

	raRain::~raRain(void)
	{
	}
};