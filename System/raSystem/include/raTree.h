#pragma once

	class RAPI raTree : public raEntity<VERTEXPOSITIONNORMALTEXTURED, true, UINT32>
	{
	public:
		raTree(raSmartPointer<raDirectX> dx, LPCSTR barkTextureFile, LPCSTR leafTextureFile, int height,
			int nSlices = 8, float r0 = 1.0f, raMatrix *pTransform = NULL);

		raTree(raSmartPointer<raDirectX> dx, raMaterial* barkMaterial, LPCSTR leafTextureFile, int height, int nSlices = 8,
			float r0 = 1.0f, raMatrix *pTransform = NULL);

		virtual bool Create();
		virtual void Destroy();

		virtual void SetupVertices();
		virtual void SetupIndices();

		virtual void SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight);
		virtual bool Render(UINT drawOrder, const raMatrix &pView, const raMatrix &pProj,
							const raLight* pLight, LPCSTR techniqueName = "");

	private:
		int AddBranchVerts(VERTEXPOSITIONNORMALTEXTURED* vertices, int n, raMatrix* pParentMatrix, int level);
		float r(int y)	{return m_r0 + y / (m_nHeightSegs - 1.0f) * (m_r1 - m_r0);}

	private:
		int			m_nLevels;
		int			m_nChildren;
		float		m_rRatio;
		float		m_lRatio;
		int			m_nHeightSegs;
		int			m_nSlices;
		float		m_l0;
		float		m_r0;
		float		m_r1;
		int			m_nMaxBranches;
		int			m_nBranches;
		raMatrix*	m_pTransform;

		raParticleSystem m_ParticleSystem;
	};