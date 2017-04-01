#include "..\include\raMain.h"
namespace System
{
	raSkyDome::raSkyDome(raSmartPointer<raDirectX> dx, int width, int height,
		float radius, bool bProcedural, bool bLighted, raMaterial* pMaterial) : raEntity(dx, pMaterial? pMaterial
			  :	(bProcedural ? (raMaterial*)new raProceduralMaterial(dx, 64, "Fbm", 4):
				((raMaterial*)new raTexturedMaterial(dx, "raEffects\\NoNormals.fx",
				bLighted?"RenderWithTextureAmbient":"RenderWithTexture",
				"raGrafik\\Skydome1.jpg"))))
	{
		m_r = radius;
		m_width = (float)width;
		m_height = (float)height;

		m_nVertices = (DWORD)(m_width * m_height);
		m_nIndices  = 6 * ((DWORD)m_width - 1) * ((DWORD)m_height - 1);

		m_DrawOder = 0;
		m_bDepthDisable = true;
		if(bProcedural) ((raProceduralMaterial*)m_pMaterials[0])->SetBias(0.6f);
	}

	void raSkyDome::SetupVertices()
	{
		m_pVertices = new VERTEXPOSITIONTEXTURED[m_nVertices];

		int n = 0;
		for (int y = 0; y < m_height; y++)
		{
			float theta;
			//Polkappe öffnen
			if(y == 0)
				theta = 0.5f * PI * (0.5f / ((float)m_height - 1));
			else
				theta = 0.5f * PI * (y / ((float)m_height - 1));
			float rsintheta = m_r * sin(theta);
			float rcostheta = m_r * cos(theta);
			for (int x = 0; x < m_width; x++)
			{
				float phi = 2 * PI * x / ((float)m_width - 1);

				m_pVertices[n].position.x = rsintheta * cos(phi);
				m_pVertices[n].position.y = rcostheta;
				m_pVertices[n].position.z = rsintheta * sin(phi);

				m_pVertices[n].texcoord.x = x / ((float)m_width  - 1);
				m_pVertices[n].texcoord.y = y / ((float)m_height - 1);

				n++;
			}
		}
	}

	void raSkyDome::SetupIndices()
	{
		m_pIndices = new UINT16[m_nIndices];

		int n = 0;
		for(int y = 0; y < m_height - 1; y++)
		{
			for(int x = 0; x < m_width - 1; x++)
			{
				m_pIndices[n + 0] = (UINT16) (y * m_width + x);
				m_pIndices[n + 1] = (UINT16) ((y + 1) * m_width + x);
				m_pIndices[n + 2] = (UINT16) (y * m_width + (x + 1));
				m_pIndices[n + 3] = (UINT16) ((y + 1) * m_width + x);
				m_pIndices[n + 4] = (UINT16) ((y + 1) * m_width + (x + 1));
				m_pIndices[n + 5] = (UINT16) (y * m_width + (x + 1));

				n+=6;
			}
		}

		m_pSubsets[0].IndexStart = 0;
		m_pSubsets[0].IndexCount = m_nIndices;
	}

	void raSkyDome::SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
			const raLight *Light)
	{
		//Die Größe der Skybox muss auf die Farplane abgestimmt sein
		//Klappt nur, wenn Mesh den Radius 1 hat - die Hälfte für eckige Skybox
		float zf =  pProj.m43/ (1 - pProj.m33) ;
		//float scale = 0.4f * zf;
		float scale = 0.04f * zf;
		raMatrix mScale;
		mScale = raMatrixScaling(raVector3(scale, scale, scale));

		raMatrix mInvView;
		D3DXMatrixInverse((D3DXMATRIX*)&mInvView, NULL, (D3DXMATRIX*)&pView);
		raMatrix worldTmp(mScale * m_World);
		worldTmp.m41 = mInvView.m41;
		worldTmp.m42 = mInvView.m42 - scale * 0.2f;
		worldTmp.m43 = mInvView.m43;

		raMatrix mWorldView, mWorldViewProjection;
		mWorldView = worldTmp * pView;
		mWorldViewProjection = mWorldView * pProj;

	   for(DWORD matNo = 0; matNo < m_numMaterials; matNo++)
		{
			m_pMaterials[matNo]->SetupPerFrame(Light);
			m_pMaterials[matNo]->SetupPerObject(worldTmp, mWorldView,
				mWorldViewProjection);
		}
	}
};