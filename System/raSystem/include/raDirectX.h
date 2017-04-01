#pragma once

//class raBuffer;

	class RAPI raDirectX : public raGrafikDriver
	{
	public:
		raDirectX(void) {  m_rondomID = timeGetTime()*rand(); raDirectX(false); }
		raDirectX(bool usedwarp);
		virtual ~raDirectX(void);

		virtual bool Init(HWND wnd);
		virtual void Exit(void);
		virtual bool Resize(void);
		virtual bool ChangeMode(void);

		virtual void Clear(raColor clearColor, UINT ClearFlag = D3D11_CLEAR_DEPTH, float Depth = 1.0f, UINT8 Stencil = 0  );
		virtual void DrawIndexed(unsigned int numTriangles);
		virtual void DrawAuto();
		virtual void Present();
		virtual void CreateScreenShot(const  raString SaveFile);

		virtual long OnKeyUp(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long OnKeyDown(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long OnClose(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long OnSize(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long OnPaint(HWND wnd, WPARAM wParam, LPARAM lParam);

		ID3D11Device					*GetDevice(void)				{ return m_pDevice; }
		IDXGISwapChain					*GetSwapChain(void)				{ return m_pSwapChain; }
		ID3D11DeviceContext				*GetImmediateContext(void)		{ return m_pImmediateContext; }
		ID3D11RenderTargetView			*GetRenderTargetView(void)		{ return m_pRenderTargetView; }
		ID3D11DepthStencilView			*GetDethStencilView(void)		{ return m_pDSV; }
		DXGI_SWAP_CHAIN_DESC			GetSwapChainDESC(void)			{ return m_swapChainDesc; }
		D3D_DRIVER_TYPE					GetDriverType(void)				{ return m_driverType; }
		D3D11_TEXTURE2D_DESC			GetDepthBufferDesc(void)		{ return m_depthBufferDesc; }
		D3D11_DEPTH_STENCIL_VIEW_DESC	GetDepthStencilViewDesc(void)	{ return m_descDSV; }
		D3D11_VIEWPORT					GetViewPort(void)				{ return m_vp; }

		IDXGIDevice					    *GetDXGIDevice(void)			{ return m_pDXGIDevice; }
		IDXGIFactory					*GetDXGIFactory(void)			{ return m_pIDXGIFactory; }
		IDXGIAdapter					*GetDXGIAdapter(void)			{ return m_pDXGIAdapter; }

	private:
		ID3D11Device					*m_pDevice;
		IDXGISwapChain					*m_pSwapChain;
		ID3D11DeviceContext				*m_pImmediateContext;
		ID3D11RenderTargetView			*m_pRenderTargetView;
		ID3D11Texture2D					*m_pDepthStencil;
		D3D_FEATURE_LEVEL				*m_featureLevel;
		ID3D11DepthStencilView			*m_pDSV;
		ID3D11RasterizerState			*m_pRS;
		D3D_DRIVER_TYPE					m_driverType;
		DXGI_SWAP_CHAIN_DESC			m_swapChainDesc;
		D3D11_TEXTURE2D_DESC			m_depthBufferDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC	m_descDSV;
		D3D11_VIEWPORT					m_vp;
		D3D11_RASTERIZER_DESC			m_RSDesc;

		bool							m_withwarp;
		UINT							m_Synce;
	private:
		IDXGIFactory*					m_pIDXGIFactory;
		IDXGIAdapter*					m_pDXGIAdapter;
		IDXGIDevice*					m_pDXGIDevice;
		IDXGIOutput*					m_pDXGIOutput;
	};