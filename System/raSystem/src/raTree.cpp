#include "..\include\raMain.h"

namespace System
{
	raTree::raTree(raSmartPointer<raDirectX> dx, LPCSTR barkTextureFile, LPCSTR leafTextureFile, int height,
			int nSlices, float r0, raMatrix *pTransform) : raEntity(dx, new raTexturedMaterial(dx.get(),
				"raEffects\\MyEngine.fx", "RenderScene",
				barkTextureFile)),
			m_ParticleSystem(dx, new raTexturedMaterial(dx.get(),
				"raEffects\\ParticleEffect.fx", "RenderParticle",
				leafTextureFile), false, 2.0f, 0, 0)
	{
		   m_nLevels = 4;
		   m_nChildren = 4;
		   m_rRatio = raGetRandFloat(0.25f, 0.75f);
		   m_lRatio = raGetRandFloat(0.5f, 0.75f);

		   m_r0 = r0;
		   m_l0 = 10.0f;

		   m_nHeightSegs = height;
		   m_nSlices = nSlices;

		   m_nMaxBranches = (int) pow((float)m_nChildren, m_nLevels + 1) - 1;
		   m_nBranches = 1;

		   m_nVertices = m_nMaxBranches * m_nSlices * m_nHeightSegs;
		   m_nIndices = m_nMaxBranches * 6 * m_nSlices * (m_nHeightSegs - 1);// nicht m_nSlices - 1, weil die Naht geschlossen wird

		   m_pTransform = pTransform;
	}
	raTree::raTree(raSmartPointer<raDirectX> dx, raMaterial* barkMaterial, LPCSTR leafTexturefilename, int height, int nSlices,
			float r0, raMatrix *pTransform) : raEntity(dx, barkMaterial),
			m_ParticleSystem(dx, new raTexturedMaterial(dx,
				"raEffects\\ParticleEffect.fx", "RenderParticle", leafTexturefilename), false, 2.0f, 0, 0)
	{
		   m_nLevels = 4;
		   m_nChildren = 4;
		   m_rRatio = raGetRandFloat(0.25f, 0.75f);
		   m_lRatio = raGetRandFloat(0.5f, 0.75f);

		   m_r0 = r0;
		   m_l0 = 10.0f;

		   m_nHeightSegs = height;
		   m_nSlices = nSlices;

		   m_nMaxBranches = (int) pow((float)m_nChildren, m_nLevels + 1) - 1;
		   m_nBranches = 1;

		   m_nVertices = m_nMaxBranches * m_nSlices * m_nHeightSegs;
		   m_nIndices = m_nMaxBranches * 6 * m_nSlices * (m_nHeightSegs - 1);// nicht m_nSlices - 1, weil die Naht geschlossen wird

		   m_pTransform = pTransform;
	}
	bool raTree::Create()
	{
		if(!raEntity::Create())
		{
			RERROR("Bei raEntity Create");
			return false;
		}
		if(!m_ParticleSystem.Create())
		{
			RERROR("Beim erstellen des ParticleSystems");
			return false;
		}

		ROK("Baum Erstellt");
		return true;
	}
	 void raTree::Destroy()
	{
		raEntity::Destroy();
		m_ParticleSystem.Destroy();
	}

	void raTree::SetupVertices()
	{
		m_pVertices =  new VERTEXPOSITIONNORMALTEXTURED[m_nVertices];

		m_nVertices = 0;

		if(m_pTransform)
		{
			m_nVertices = AddBranchVerts((VERTEXPOSITIONNORMALTEXTURED*)m_pVertices,
			m_nVertices, m_pTransform, m_nLevels);
		}
		else
		{
			raMatrix identity;
			identity = raMatrixIdentity();
			m_nVertices = AddBranchVerts((VERTEXPOSITIONNORMALTEXTURED*)m_pVertices,
				m_nVertices, &identity, m_nLevels);
		}
	}

	int raTree::AddBranchVerts(VERTEXPOSITIONNORMALTEXTURED* vertices, int n, raMatrix* pParentMatrix, int level)
	{
		if(level == 0)
		{
			m_r1 = 0; //Ende des letzten Asts schlieﬂen

			raVector3 position(0, m_l0 * (m_nHeightSegs-1), 0);
			raVector4 t;
			D3DXVec3Transform((D3DXVECTOR4*)&t, (D3DXVECTOR3*)&position, (D3DXMATRIX*)pParentMatrix);
			m_ParticleSystem.AddParticle(t.x, t.y, t.z);
		}
		else
			m_r1 = m_rRatio * m_r0;

		for(int y = 0; y < m_nHeightSegs; y++)
		{
			for(int phi = 0; phi < m_nSlices; phi++)
			{
				float r0 = r(y);
				float phi0 = 2 * PI * phi / (m_nSlices-1);

				raVector3 position(
					(float) r0 * sin(phi0),
					m_l0 * y,
					(float) r0 * cos(phi0));

				raVector3 normal(
					(float)position.x,
					0.0f,
					(float)position.z);

				D3DXVec3TransformCoord((D3DXVECTOR3*)&vertices[n].position, (D3DXVECTOR3*)&position,
					(D3DXMATRIX*)pParentMatrix);

				D3DXVec3TransformNormal((D3DXVECTOR3*)&vertices[n].normal, (D3DXVECTOR3*)&normal,
					(D3DXMATRIX*)pParentMatrix);

				vertices[n].texcoord.x = phi / (m_nSlices - 1.0f);
				vertices[n].texcoord.y = y / (m_nHeightSegs - 1.0f);

				n+=1;
			}
		}

		if(level > 0)
		{
			raMatrix mtrans;
			D3DXMatrixTranslation((D3DXMATRIX*)&mtrans, 0, m_l0 * (m_nHeightSegs-1), 0);

			int nChildren = raGetRandInt(3, m_nChildren);

			float pitch = PI / 4.0f;
			float yaw = 2.0f * PI / nChildren;

			raMatrix  mrot1, mrot2;
			mrot1 = raMatrixRotationZ(pitch);

			raMatrix m0, m1, m2, m3, m4;
			D3DXMatrixScaling((D3DXMATRIX*)&m0, m_rRatio, m_lRatio, m_rRatio);

			for(int child = 0; child < nChildren; child++)
			{
				mrot2 = raMatrixRotationY(child * yaw);

				m1 = m0 * mrot1;
				m2 = m1 * mrot2;
				m3 = m2 * mtrans;
				m4 = m3 * (*pParentMatrix);

				n = AddBranchVerts(vertices, n, &m4, level - 1);
			}
			m_nBranches += nChildren;
		}
		return n;
	}

	void raTree::SetupIndices()
	{
		m_pIndices = new UINT32[m_nIndices];

		m_nIndices = 0;
		for(int branch= 0; branch < m_nBranches; branch++)
		{
			int offs = branch * m_nSlices * m_nHeightSegs;
			for(int y = 0; y < m_nHeightSegs - 1; y++)
			{
				//Naht schliessen - daher nicht bis 360 - 360 / m_nSlices, sondern bis 360
				for(int phi = 0; phi < m_nSlices; phi++)
				{
					m_pIndices[m_nIndices + 0] = offs + y * m_nSlices + phi;
					m_pIndices[m_nIndices + 1] = offs + y * m_nSlices +
						(phi + 1) % m_nSlices;
					m_pIndices[m_nIndices + 2] = offs + (y + 1) * m_nSlices + phi;
					m_pIndices[m_nIndices + 3] = offs + (y + 1) * m_nSlices + phi;
					m_pIndices[m_nIndices + 4] = offs + y * m_nSlices +
						(phi + 1) % m_nSlices;
					m_pIndices[m_nIndices + 5] = offs + (y + 1) * m_nSlices +
						(phi + 1) % m_nSlices; ;

					m_nIndices+=6;
				}
			}
		}
		m_pSubsets[0].IndexStart = 0;
		m_pSubsets[0].IndexCount = m_nIndices;
	}

	void raTree::SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight)
	{
		raEntity::SetupEffectVariables(pView, pProj, pLight);
		m_ParticleSystem.SetupEffectVariables(pView, pProj, pLight);
	}

	bool raTree::Render(UINT drawOrder, const raMatrix &pView, const raMatrix &pProj,
							const raLight* pLight, LPCSTR techniqueName)
	{
		raEntity::Render(drawOrder,
			pView, pProj, pLight, techniqueName);
		m_ParticleSystem.Render(drawOrder,
			pView, pProj, pLight, techniqueName);

		return true;
	}
};