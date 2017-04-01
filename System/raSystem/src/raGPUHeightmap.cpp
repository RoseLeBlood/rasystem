#include "..\include\raMain.h"
namespace System
{
	raGPUHeightmap::raGPUHeightmap(raDirectX *dx, int width, int depth,
								   raMaterial *mat, float tileSize)
								   : raHoehenfeld(dx, width, depth, mat, tileSize)
	{
	}

	float raGPUHeightmap::h(float x, float z)
	{
		return 0;
	}
	raVector3 raGPUHeightmap::normal(float x, float z)
	{
		return raVector3(0.0f, 1.0f, 0.0f);
	}

	//--------------------
	raGPUHeightmapMaterial::raGPUHeightmapMaterial(raDirectX *dx, LPCSTR effectFile,
												   LPCSTR techniqueName, LPCSTR texture,
												   LPCSTR heightmap, LPCSTR heightmapBump)
												   : raTexturedMaterial(dx, effectFile, techniqueName,
												   texture)
	{
		m_HeightTextureFileName = heightmap;
		m_BumMapFileName = heightmapBump;

		m_pRVHeight = NULL;
		m_pRVBump = NULL;
	}
	bool raGPUHeightmapMaterial::Create()
	{
		raTexturedMaterial::Create();

		raResource::Instance().CreateTextureFromFile(m_dx,
			m_HeightTextureFileName, &m_pRVHeight);

		raResource::Instance().CreateTextureFromFile(m_dx,
			m_BumMapFileName, &m_pRVBump);

		m_ptxHeight = m_pEffect->GetVariableByName("g_HeightTexture")->AsShaderResource();
		m_ptxBumpMap = m_pEffect->GetVariableByName("g_NormalMap")->AsShaderResource();

		return true;
	}
	bool raGPUHeightmapMaterial::Destroy()
	{
		raTexturedMaterial::Destroy();

		if(m_pRVHeight) m_pRVHeight->Release();
		if(m_pRVBump) m_pRVBump->Release();

		return true;
	}
	void raGPUHeightmapMaterial::Setup()
	{
		raTexturedMaterial::Setup();

		if(m_ptxHeight) m_ptxHeight->SetResource(m_pRVHeight);
		if(m_ptxBumpMap) m_ptxBumpMap->SetResource(m_pRVBump);
	}
};