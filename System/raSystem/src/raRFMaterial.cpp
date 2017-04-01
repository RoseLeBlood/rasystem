#include "..\include\raMain.h"

namespace System
{
	raRFMaterial::raRFMaterial(raSmartPointer<raDirectX> dx, LPCSTR techniqueName, int envMapWidth, int envMapHeight)
							 : raRTTMaterial(dx, "raEffects\\Reflection.fx",
							 techniqueName, D3D11_SRV_DIMENSION_TEXTURECUBE, envMapWidth, envMapHeight,
							 1, DXGI_FORMAT_R16G16B16A16_FLOAT,
							 "g_cubeMapTexture", 6)
	{
		m_r = 0.9f; // Verhältnis der Brechungsindizes
		m_pow = 5.0f;
		m_reflectivity = 0.5f;
	}
	bool raRFMaterial::Create()
	{
		raRTTMaterial::Create();

		m_pR = m_pEffect->GetVariableByName("r")->AsScalar();
		m_pR3 = m_pEffect->GetVariableByName("r3")->AsVector();
		m_pFres0 = m_pEffect->GetVariableByName("fres0")->AsScalar();
		m_pFresPow = m_pEffect->GetVariableByName("fresPow")->AsScalar();
		m_pRefl = m_pEffect->GetVariableByName("reflectivity")->AsScalar();

		ROK("Reflectirendes Material erstellt");
		return true;
	}

	void raRFMaterial::Setup()
	{
		raRTTMaterial::Setup();

		if(m_pR)    m_pR->SetFloat(m_r);
		if(m_pR3)
		m_pR3->SetFloatVector(raVector3(m_r, m_r + 0.01f, m_r + 0.02f));

		float fres0 = ((1 - m_r) * (1 - m_r)) / ((1 + m_r) * (1 + m_r));
		if(m_pFres0)    m_pFres0->SetFloat(fres0);
		if(m_pFresPow)  m_pFresPow->SetFloat(m_pow);
		if(m_pRefl)     m_pRefl->SetFloat(m_reflectivity);
	}
};