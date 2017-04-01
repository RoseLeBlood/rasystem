#pragma once

// Material + Rendertarget class ( raRenderScreen )

	class RAPI raRTTMaterial : public raTexturedMaterial
	{
	public:
		raRTTMaterial(raSmartPointer<raDirectX>, LPCSTR effectFile = "raEffects\\NoNormals.fx",
			LPCSTR technique = "RenderWithTexture", D3D11_SRV_DIMENSION viewDimension = D3D11_SRV_DIMENSION_TEXTURE2D,
			int envMapWidth = 128, int enyMapHeight = 128, int mipLevels = 1, DXGI_FORMAT fmt = DXGI_FORMAT_R16G16B16A16_FLOAT,
			LPCSTR effectVariableName = "g_MeshTexture", UINT arraySize = 1);

		virtual bool Create();
		virtual bool Destroy();

		raRenderScreen& GetRenderScreen()		{ return m_renderScreen; }
		void GetSizeFromBackBuffer(const DXGI_SURFACE_DESC* desc);

	protected:
		D3D11_SRV_DIMENSION m_ViewDimension;
		ID3D11Texture2D*	m_pRenderTexture;
		raRenderScreen		m_renderScreen;
	};