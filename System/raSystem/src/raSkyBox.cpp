#include "..\include\raMain.h"

namespace System
{
	raSkyBox::raSkyBox(raSmartPointer<raDirectX> dx, raMaterial* pMaterial) : raEntity(dx, pMaterial)
	{
		m_nVertices = 8;
		m_nIndices = 36;

		m_DrawOder = 0;
		m_bDepthDisable = true;
	}
	raSkyBox::raSkyBox(raSmartPointer<raDirectX> dx, LPCSTR texFileName) :
	raEntity(dx, new raTexturedMaterial(dx.get(), "raEffects\\SkyBoxEffect.fx", "RenderSkyBox",
			 texFileName, "g_SkyboxTexture"))
	{
		m_nVertices = 8;
		m_nIndices = 36;

		m_DrawOder = 0;
		m_bDepthDisable = true;
	}
	void raSkyBox::SetupVertices()
	{
		m_pVertices = new CUBEMAPVERTEX[m_nVertices];
		m_pVertices[0].position.x = -1.0f;
		m_pVertices[0].position.y = -1.0f;
		m_pVertices[0].position.z = -1.0f;

		m_pVertices[1].position.x = -1.0f;
		m_pVertices[1].position.y = 1.0f;
		m_pVertices[1].position.z = -1.0f;

		m_pVertices[2].position.x = 1.0f;
		m_pVertices[2].position.y = -1.0f;
		m_pVertices[2].position.z = -1.0f;

		m_pVertices[3].position.x = 1.0f;
		m_pVertices[3].position.y = 1.0f;
		m_pVertices[3].position.z = -1.0f;

		m_pVertices[4].position.x = -1.0f;
		m_pVertices[4].position.y = -1.0f;
		m_pVertices[4].position.z = 1.0f;

		m_pVertices[5].position.x = -1.0f;
		m_pVertices[5].position.y = 1.0f;
		m_pVertices[5].position.z = 1.0f;

		m_pVertices[6].position.x = 1.0f;
		m_pVertices[6].position.y = -1.0f;
		m_pVertices[6].position.z = 1.0f;

		m_pVertices[7].position.x = 1.0f;
		m_pVertices[7].position.y = 1.0f;
		m_pVertices[7].position.z = 1.0f;

		for(UINT i = 0; i < m_nVertices; i++)
		{
			m_pVertices[i].texcoord = m_pVertices[i].position;
		}
	}
	void raSkyBox::SetupIndices()
	{
		m_pIndices = new UINT16[m_nIndices];

		//vorn
		m_pIndices[0] = 0;
		m_pIndices[1] = 2;
		m_pIndices[2] = 1;
		m_pIndices[3] = 1;
		m_pIndices[4] = 2;
		m_pIndices[5] = 3;

		//hinten
		m_pIndices[6] = 4;
		m_pIndices[7] = 5;
		m_pIndices[8] = 6;
		m_pIndices[9] = 5;
		m_pIndices[10] = 7;
		m_pIndices[11] = 6;

		//links
		m_pIndices[12] = 4;
		m_pIndices[13] = 1;
		m_pIndices[14] = 5;
		m_pIndices[15] = 4;
		m_pIndices[16] = 0;
		m_pIndices[17] = 1;

		//rechts
		m_pIndices[18] = 2;
		m_pIndices[19] = 7;
		m_pIndices[20] = 3;
		m_pIndices[21] = 2;
		m_pIndices[22] = 6;
		m_pIndices[23] = 7;

		//unten
		m_pIndices[24] = 0;
		m_pIndices[25] = 4;
		m_pIndices[26] = 6;
		m_pIndices[27] = 0;
		m_pIndices[28] = 6;
		m_pIndices[29] = 2;

		//oben
		m_pIndices[30] = 1;
		m_pIndices[31] = 7;
		m_pIndices[32] = 5;
		m_pIndices[33] = 1;
		m_pIndices[34] = 3;
		m_pIndices[35] = 7;

		m_pSubsets[0].IndexStart = 0;
		m_pSubsets[0].IndexCount = m_nIndices;
	}
	void raSkyBox::SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight)
	{
		float zf = pProj.m43 / ( 1 - pProj.m33);
		float scale = 0.04f * zf;
		raMatrix mScale;
		mScale = raMatrixScaling(raVector3(scale, scale,scale));

		raMatrix worldTmp(mScale * m_World);
		raMatrix viewTmp(pView);

		viewTmp.m41 = 0;
		viewTmp.m42 = 0;
		viewTmp.m43 = 0;

		raMatrix mWorldView, mWorldViewProjection;
		mWorldView = worldTmp * viewTmp;
		mWorldViewProjection = mWorldView * pProj;

		for (DWORD matNo = 0; matNo < m_numMaterials; matNo++)
		{
			m_pMaterials[matNo]->SetupPerFrame(pLight);
			m_pMaterials[matNo]->SetupPerObject(worldTmp, mWorldView, mWorldViewProjection);
		}
	}
};