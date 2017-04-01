#pragma once

	class RAPI raRenderScreen
	{
		friend class raSceneManager;
	public:
		raRenderScreen(raSmartPointer<raDirectX> dx, int width, int height, int mipLevels = 1,
			DXGI_FORMAT fmt = DXGI_FORMAT_R16G16B16A16_FLOAT, UINT arraySize = 1);

		ID3D11Texture2D* CreateRenderTarget();
		virtual void	 Destroy();

		raDirectX*		 GetDirectX();
		ID3D11Texture2D* GetTexture();
		DXGI_FORMAT		 GetFormat();
		int				 GetMipLevels();
		UINT			 GetArraySize();

		void		     GetSizeFromBackBuffer(const DXGI_SURFACE_DESC* pBackBufferDesc);

	protected:
		bool CreateDepthTexture(D3D11_TEXTURE2D_DESC& desc);
		bool CreateTexture(D3D11_TEXTURE2D_DESC& desc);
		bool CreateDepthStencilView();
		bool CreateRenderTargetView();
	protected:
		raSmartPointer<raDirectX>		 m_dx;
		int				 m_Width;
		int				 m_Height;
		int				 m_MipLevels;
		unsigned int	 m_arraySize;
		DXGI_FORMAT		 m_Format;

		ID3D11RenderTargetView*		m_pRTView;
		ID3D11DepthStencilView*		m_pDSView;

		ID3D11Texture2D*			m_pRenderTarget;
		ID3D11Texture2D*			m_pDepth;
	};