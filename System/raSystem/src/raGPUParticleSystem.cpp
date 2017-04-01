#include "..\include\raMain.h"

namespace System
{
	raGPUParticleSystem::raGPUParticleSystem(raDirectX *dx,
		    LPCSTR ParticleTexture,
			LPCSTR EffectFile,
			LPCSTR Technique) : raVisual(dx, true, true)
	{
		raGPUParticleSystem(dx, new raTexturedMaterial(dx, EffectFile, Technique, ParticleTexture));
	}
	raGPUParticleSystem::raGPUParticleSystem(raDirectX *dx, raMaterial *ParticleMat) :
		raVisual(dx,  true, true)
	{
		this->m_bDepthDisable = true;
		m_mat = ParticleMat;

		this->m_numMaterials = 1;
		this->m_pMaterials = new raMaterial*[m_numMaterials];
		m_pMaterials[0] = ParticleMat;

		this->m_Particles.resize(MAXPARTICLES);
		for(int i = 0; i < MAXPARTICLES; i++)
		{
			m_Particles[i].position = raVector3(raGetRandFloat(-10.0f, 10.0f),
				raGetRandFloat(-10.0f, 10.0f), raGetRandFloat(-10.0f, 10.0f));
			m_Particles[i].velocity = 2.0f * raVector3(raGetRandFloat(-1.0f, 1.0f),
				raGetRandFloat(-1.0f, 1.0f), raGetRandFloat(-1.0f, 1.0f));

			for(int j = 0; j < 3; j++) m_Particles[i].color[j] =
				raGetRandFloat(0.0f, 1.0f);
		}

		m_pCS = new raCSIOBuffer(dx, "./raEffects\\ParticleGPU.hlsl", "CSCompute", (const raFloat*)&m_Particles[0],
			sizeof(PARTICLEVERTEX) / 4 * MAXPARTICLES);
	}
	void raGPUParticleSystem::SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight)
	{
		raVisual::SetupEffectVariables(pView, pProj, pLight);

		ID3DX11Effect* pEffect = m_pMaterials[0]->GetEffect();
		ID3DX11EffectMatrixVariable* pmInvView_ParticleSystem = pEffect->
			GetVariableByName("g_mInvView")->AsMatrix();

		raMatrix mInvView = raMatrixInvert(pView);
		pmInvView_ParticleSystem->SetMatrix((float*)&mInvView);
	}

	bool raGPUParticleSystem::RenderMesh(LPCSTR techniqueName)
	{
		m_pMaterials[0]->Setup();

		UINT Strides[1];
		UINT Offsets[1];

		Strides[0] = GetStrideSize();
		Offsets[0] = 0;

		ID3D11Buffer* aBuffers[1] = { m_pCS->GetBuffer() };
		m_dx->GetImmediateContext()->IASetVertexBuffers(0, 1, aBuffers, Strides, Offsets);

		ID3DX11EffectTechnique* pTech =
			m_pMaterials[0]->GetEffectTechnique(m_mat->GetTechniqueName().c_str());
		HRESULT hr = pTech->GetPassByIndex(0)->Apply(0, m_dx->GetImmediateContext());

		if(FAILED(hr))
		{
			RERROR_DX11("ID3DX11EffectTechnique::GetPassByIndex::Apply", hr);
			return false;
		}

		m_dx->GetImmediateContext()->IASetPrimitiveTopology(this->GetPrimitiveTopology(0));
		m_dx->GetImmediateContext()->Draw(MAXPARTICLES, 0);

		ID3D11Buffer* ppVBNULL[1] = { 0 };
		m_dx->GetImmediateContext()->IASetVertexBuffers(0, 1, ppVBNULL, Strides, Offsets);

		return true;
	}
};