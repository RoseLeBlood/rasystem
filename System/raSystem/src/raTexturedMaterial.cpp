#include "..\include\raMain.h"

namespace System
{
	raTexturedMaterial::raTexturedMaterial(raSmartPointer<raDirectX> dx, LPCSTR EffectFile, LPCSTR Technique,
										   LPCSTR textureFileName, LPCSTR effectVariable) :
										   raMaterial(dx, EffectFile, Technique),
										   m_TextureFileName(textureFileName)
	{
		m_pRV = NULL;
		m_ptxDiffuseVariable = NULL;
		m_effectVariableName = effectVariable;
	}
	bool raTexturedMaterial::Create()
	{
		raMaterial::Create();

		HRESULT hr;
		hr = raResource::Instance().CreateTextureFromFile(m_dx, m_TextureFileName, &m_pRV);

		m_ptxDiffuseVariable = m_pEffect->GetVariableByName(m_effectVariableName)->AsShaderResource();
		return true;
	};
	bool raTexturedMaterial::Destroy()
	{
		raMaterial::Destroy();
		if(m_pRV) m_pRV->Release();
		return true;
	}
	void raTexturedMaterial::Setup()
	{
		raMaterial::Setup();
		if(m_ptxDiffuseVariable)
			m_ptxDiffuseVariable->SetResource(m_pRV);
	}
	void raTexturedMaterial::GenerateMips()
	{
		m_dx->GetImmediateContext()->GenerateMips(m_pRV);
	}
};