#include "..\include\raMain.h"

namespace System
{
	raTesselatinObject::raTesselatinObject(raDirectX *dx) : raEntity(dx,
		new raProceduralMaterial(dx, 64, "raEffects\\Terrain.fx", "Terrain"))
	{
		m_nVertices = 1;
		m_TesselationsFactor = 10;
	}
	void raTesselatinObject::SetupVertices()
	{
		m_pVertices = new VERTEXPOSITION[m_nVertices];

		m_pVertices[0].position.x = 0.0f;
		m_pVertices[0].position.y = 0.0f;
		m_pVertices[0].position.z = 0.0f;
	}
	void raTesselatinObject::SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight)
	{
		raEntity::SetupEffectVariables(pView, pProj, pLight);

		ID3DX11Effect * pEffect = GetMaterial(0)->GetEffect();

		ID3DX11EffectScalarVariable* pTeFactor = pEffect->GetVariableByName("g_fTessellationFactor")->AsScalar();

		if(pTeFactor)
			pTeFactor->SetFloat(m_TesselationsFactor);
	}
	bool raTesselatinObject::RenderMesh(LPCSTR techniqueName)
	{
		m_dx->GetImmediateContext()->IASetInputLayout(m_pVertexLayout);
		m_dx->GetImmediateContext()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);

		UINT Strides[1];
		UINT Offsets[1];

		Strides[0] = GetStrideSize();
		Offsets[0] = 0;

		m_dx->GetImmediateContext()->IASetVertexBuffers(0, 1, &m_pVB, Strides, Offsets);

		m_pMaterials[0]->Setup();

		ID3DX11EffectTechnique* pTechnique = m_pMaterials[0]->GetEffectTechnique();

		if(pTechnique)
		{
			D3DX11_TECHNIQUE_DESC techDesc;
			pTechnique->GetDesc(&techDesc);

			for(UINT p = 0; p < techDesc.Passes; ++p)
			{
				pTechnique->GetPassByIndex(p)->Apply(0, m_dx->GetImmediateContext());
				m_dx->GetImmediateContext()->Draw(m_nVertices, 0);
			}
		}
		return true;
	}
};