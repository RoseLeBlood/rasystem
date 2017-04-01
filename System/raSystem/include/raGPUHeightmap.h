#pragma once

	class RAPI raGPUHeightmap : public raHoehenfeld
	{
	public:
		raGPUHeightmap(raDirectX *dx, int width, int depth, raMaterial *mat, float tileSize = 1.0f);

	private:
		virtual float h(float x, float z); // w = Pixel Format ( 1024 x 1024 )
		virtual raVector3 normal(float x, float z); // Normale
	};

	//----------------------------------
	class RAPI raGPUHeightmapMaterial : public raTexturedMaterial
	{
	public:
		raGPUHeightmapMaterial(raDirectX *dx, LPCSTR effectFile = "raEffects\\HeightmapEffect.fx",
							   LPCSTR techniqueName = "RenderWithHeightMapAndBumpMap",
							   LPCSTR texture = "raGrafik\\Forest Flood.jpg",
							   LPCSTR heightmap = "raGrafik\\Heightmap.dds",
							   LPCSTR heightmapBump = "raGrafik\\HeightmapNormals.dds");

		virtual bool Create();
		virtual bool Destroy();
		virtual void Setup();

	protected:
		LPCSTR m_HeightTextureFileName;
		LPCSTR m_BumMapFileName;

		ID3D11ShaderResourceView*	m_pRVHeight;
		ID3D11ShaderResourceView*	m_pRVBump;

		ID3DX11EffectShaderResourceVariable*	m_ptxHeight;
		ID3DX11EffectShaderResourceVariable*	m_ptxBumpMap;
	};