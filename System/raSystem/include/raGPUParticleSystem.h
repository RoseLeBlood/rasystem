#pragma once

	class RAPI raGPUParticleSystem : public raVisual
	{
	public:
		static const int MAXPARTICLES = 100;
	public:
		raGPUParticleSystem(raDirectX *dx, LPCSTR ParticleTexture = "raGrafik\\particlered.bmp",
			LPCSTR EffectFile = "raEffects\\ParticleEffect.fx",
			LPCSTR Technique = "RenderParticle");

		raGPUParticleSystem(raDirectX *dx, raMaterial *ParticleMat);

		~raGPUParticleSystem(void) { SAFE_DELETE(m_pCS); }

		virtual bool CreateMesh()  { return m_pCS->Create(); }
		virtual bool DestroyMesh() { m_pCS->Destroy(); return true; }

        virtual void SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight);

		virtual bool RenderMesh(LPCSTR techniqueName = "");

		virtual bool Update(float fTime, float fRunTime)
		{
			return m_pCS->RunCS(MAXPARTICLES, 1, 1);
		}

		//IMesh-Funktionen
        virtual UINT GetNumSubsets(){return 1;}
        virtual SDKMESH_SUBSET* GetSubset(UINT n){return NULL;}

		virtual const D3D11_INPUT_ELEMENT_DESC* GetVertexLayout(){return PARTICLEVERTEX::GetVertexLayout();}
		virtual UINT GetNumElements(){return PARTICLEVERTEX::GetNumElements();}

		virtual ID3D11Buffer* GetVertexBuffer(){return m_pCS->GetBuffer();}
        virtual ID3D11Buffer* GetIndexBuffer(){return NULL;}
		virtual UINT GetStrideSize(){return sizeof(PARTICLEVERTEX);}

        virtual DXGI_FORMAT GetIndexBufferFormat(){return DXGI_FORMAT_R32_TYPELESS;}
		virtual D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(UINT n)
		{
			return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		}

		//Dummy-Implementierung
		virtual bool Intersects(const raVector3* pRayPos,
			const raVector3* pRayDir,
			float* pDist){return false;}
	protected:
		raCSIOBuffer*					m_pCS;
		std::vector<PARTICLEVERTEX>		m_Particles;
		raMaterial						*m_mat;
	};