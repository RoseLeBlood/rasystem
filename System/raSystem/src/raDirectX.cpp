#include "..\include\raMain.h"

namespace System
{
	D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	raDirectX::raDirectX(bool usedwarp) : raGrafikDriver()
	{
		m_pDevice = NULL;
		m_pSwapChain = NULL;
		m_pImmediateContext = NULL;
		m_pRenderTargetView = NULL;
		m_standby = false;
		m_pDXGIOutput = NULL;
		m_withwarp = usedwarp;
	}

	raDirectX::~raDirectX(void)
	{
	}
	bool raDirectX::Init(HWND wnd)
	{
		RECT rc;
		m_clientWindow = wnd;

		GetClientRect(m_clientWindow, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		m_DriverName = "DirectX11";

		ZeroMemory( &m_swapChainDesc, sizeof( m_swapChainDesc ) );
		m_swapChainDesc.BufferCount = 1;
		m_swapChainDesc.BufferDesc.Width = width;
		m_swapChainDesc.BufferDesc.Height = height;
		m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_swapChainDesc.BufferDesc.RefreshRate.Numerator = raConfigLoader::GetConfig().vsync ? 60 : 0;
		m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_swapChainDesc.OutputWindow =  m_clientWindow;
		m_swapChainDesc.SampleDesc.Count	= raConfigLoader::GetConfig().MultiSamplingCount;
		m_swapChainDesc.SampleDesc.Quality = raConfigLoader::GetConfig().MultiSamplingQualitet;

		m_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		m_swapChainDesc.Windowed = !raConfigLoader::GetConfig().FullScreen;
		m_IsFullScreen = raConfigLoader::GetConfig().FullScreen;

		HRESULT hr = S_OK;

		if(m_withwarp)
		{
			for( int driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
			{
				m_driverType = driverTypes[driverTypeIndex];

				hr = D3D11CreateDeviceAndSwapChain(NULL,m_driverType, NULL, 0,
								featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &m_swapChainDesc, &m_pSwapChain, &m_pDevice, m_featureLevel,
								&m_pImmediateContext);

				if( SUCCEEDED( hr ) )
					break;
			}
		}
		else
		{
			RINFO("Verwende nur Hardware Treiber");

			m_driverType = D3D_DRIVER_TYPE_HARDWARE;
			hr = D3D11CreateDeviceAndSwapChain( NULL ,D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
								featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &m_swapChainDesc, &m_pSwapChain, &m_pDevice, m_featureLevel,
								&m_pImmediateContext);
		}

		if(FAILED(hr))
		{
			RERROR_DX11("D3D11CreateDeviceAndSwapChain", hr);
			return false;
		}

		ROK("D3D11 Device und SwapChain erstellt");

		// set default render state to msaa enabled
		ZeroMemory(&m_RSDesc, sizeof(m_RSDesc));
		m_RSDesc.FillMode = D3D11_FILL_SOLID;
		m_RSDesc.CullMode = D3D11_CULL_BACK;
		m_RSDesc.FrontCounterClockwise = false;
		m_RSDesc.DepthBias = 0;
		m_RSDesc.DepthBiasClamp = 0.0f;
		m_RSDesc.SlopeScaledDepthBias = 0.0f;
		m_RSDesc.DepthClipEnable = true;
		m_RSDesc.ScissorEnable = false;
		m_RSDesc.MultisampleEnable = true;
		m_RSDesc.AntialiasedLineEnable = false;

		hr = m_pDevice->CreateRasterizerState(&m_RSDesc, &m_pRS);
        if ( FAILED( hr ) )
        {
            RERROR_DX11("CreateRasterizerState", hr);
			return false;
        }
		this->m_pImmediateContext->RSSetState(m_pRS);

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		D3D11_TEXTURE2D_DESC backBufferDesc;

		hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
		if( FAILED( hr ) )
		{
			RERROR_DX11("GetBuffer", hr);
			return false;
		}

		// Get back-buffer surface description
		pBackBuffer->GetDesc(&backBufferDesc);

		hr = m_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
		pBackBuffer->Release();
		if( FAILED( hr ) )
		{
			RERROR_DX11("CreateRenderTargetView", hr);
			return false;
		}

		ROK("Render Target View erstellt");

		// Create depth stencil texture
		ZeroMemory( &m_depthBufferDesc, sizeof(m_depthBufferDesc) );
		m_depthBufferDesc.Width = backBufferDesc.Width;
		m_depthBufferDesc.Height = backBufferDesc.Height;
		m_depthBufferDesc.MipLevels = 1;
		m_depthBufferDesc.ArraySize = 1;
		m_depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
		m_depthBufferDesc.SampleDesc = backBufferDesc.SampleDesc;
		m_depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		m_depthBufferDesc.CPUAccessFlags = 0;
		m_depthBufferDesc.MiscFlags = 0;

		m_Synce = raConfigLoader::GetConfig().vsync ? 1 : 0;

		if(FAILED(hr = m_pDevice->CreateTexture2D(&m_depthBufferDesc, NULL, &m_pDepthStencil )))
		{
			RERROR_DX11("CreateTexture2D", hr);
			return false;
		}
		ROK("DepthStencilTexture erstellt");

		// Create the depth stencil view
		ZeroMemory( &m_descDSV, sizeof(m_descDSV) );

		m_descDSV.Format = m_depthBufferDesc.Format;
		if (m_depthBufferDesc.SampleDesc.Count > 1) m_descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		else m_descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		m_descDSV.Texture2D.MipSlice = 0;

		if(FAILED(hr = m_pDevice->CreateDepthStencilView(m_pDepthStencil, &m_descDSV, &m_pDSV )))
		{
			RERROR_DX11("CreateDepthStencilView", hr);
			return false;
		}
		// bind the render target
		m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDSV );

		ROK("DepthStencilView erstellt und gebunden");

		// Setup the viewport
		m_vp.Width = (FLOAT)width;
		m_vp.Height = (FLOAT)height;
		m_vp.MinDepth = 0.0f;
		m_vp.MaxDepth = 1.0f;
		m_vp.TopLeftX = 0;
		m_vp.TopLeftY = 0;
		m_pImmediateContext->RSSetViewports( 1, &m_vp );

		hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&m_pDXGIDevice);
		if(FAILED(hr))
		{
			RERROR_DX11("ID3D11Device::QueryInterface (IDXGIDevice)", hr);
			return false;
		}

		hr = m_pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&m_pDXGIAdapter);
		if(FAILED(hr))
		{
			RERROR_DX11("IDXGIIDevice::GetParent (IDXGIAdapter)", hr);
			return false;
		}

		hr = m_pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_pIDXGIFactory);
		if(FAILED(hr))
		{
			RERROR_DX11("IDXGIAdapter::GetParent (IDXGIFactory)", hr);
			return false;
		}

		m_pIDXGIFactory->MakeWindowAssociation(wnd, 0);

		m_pSwapChain->GetContainingOutput(&m_pDXGIOutput);

		ROK("raDirectX erfolgreich initialisiert");

		m_standby = false;
		return true;
	}
	void raDirectX::Exit()
	{
		if(m_pImmediateContext)
		{
			m_pImmediateContext->ClearState();
			m_pImmediateContext->Flush();
		}

		SAFE_RELEASE(m_pRenderTargetView);
		SAFE_RELEASE(m_pSwapChain);
		SAFE_RELEASE(m_pImmediateContext);
		SAFE_RELEASE(m_pDevice);
		SAFE_RELEASE(m_pDSV);
		SAFE_RELEASE(m_pDXGIDevice);
		SAFE_RELEASE(m_pIDXGIFactory);
		SAFE_RELEASE(m_pDXGIAdapter);

		m_standby = true;
	}
	void raDirectX::Clear(raColor clearColor, UINT ClearFlag, float Depth, UINT8 Stencil  )
	{
		if(m_standby || !m_pImmediateContext ) return;

		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
		m_pImmediateContext->ClearDepthStencilView(m_pDSV, ClearFlag, Depth, Stencil );
	}
	void raDirectX::DrawAuto()
	{
		if(m_standby || !m_pImmediateContext ) return;
		m_pImmediateContext->DrawAuto();
	}
	void raDirectX::DrawIndexed(unsigned int numTriangles)
	{
		if(m_standby || !m_pImmediateContext ) return;
		m_pImmediateContext->DrawIndexed(numTriangles, 0, 0);
	}
	void raDirectX::Present()
	{
		if(m_standby || !m_pImmediateContext ) return;

        m_pSwapChain->Present(m_Synce, 0);
	}
	bool raDirectX::ChangeMode()
	{
		HRESULT hr = S_OK;

		if(m_standby || !m_pImmediateContext ) return false;

		m_standby = true;

        m_IsFullScreen = !m_IsFullScreen;
        if (m_IsFullScreen)
		{
            DXGI_MODE_DESC adapterDesktopDisplayMode =
            {
                800, 600, { 60, 1 }, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB
            };
			adapterDesktopDisplayMode.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			adapterDesktopDisplayMode.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

			DXGI_OUTPUT_DESC Desc;
			m_pDXGIOutput->GetDesc( &Desc );
			adapterDesktopDisplayMode.Width = Desc.DesktopCoordinates.right - Desc.DesktopCoordinates.left;
			adapterDesktopDisplayMode.Height = Desc.DesktopCoordinates.bottom - Desc.DesktopCoordinates.top;

			m_swapChainDesc.BufferDesc = adapterDesktopDisplayMode;
        }
		else
		{
            RECT r;
			GetWindowRect(this->GetClientWindow(), &r);

            m_swapChainDesc.BufferDesc.Height = r.bottom;
            m_swapChainDesc.BufferDesc.Width = r.right;
        }

		if(m_IsFullScreen == false)
			m_pSwapChain->SetFullscreenState(false, NULL);
		else
			m_pSwapChain->SetFullscreenState(false, m_pDXGIOutput);

		m_pSwapChain->ResizeBuffers(
					m_swapChainDesc.BufferCount,
					m_swapChainDesc.BufferDesc.Width,
					m_swapChainDesc.BufferDesc.Height,
					m_swapChainDesc.BufferDesc.Format,
					m_swapChainDesc.Flags);

		return true;
	}
	bool raDirectX::Resize(void)
	{
		RECT rcCurrentClient;
		GetWindowRect(m_clientWindow, &rcCurrentClient);

		UINT width = rcCurrentClient.right - rcCurrentClient.left;
		UINT height = rcCurrentClient.bottom - rcCurrentClient.top;

		m_pSwapChain->GetFullscreenState((BOOL*)&m_IsFullScreen, NULL);
		if(!m_pDevice && m_IsFullScreen) return false;

		m_standby = true;

		m_pImmediateContext->OMSetRenderTargets(0, 0, 0);

		SAFE_RELEASE(m_pRenderTargetView);

		m_swapChainDesc.BufferDesc.Width = width;
		m_swapChainDesc.BufferDesc.Height = height;

		m_pSwapChain->ResizeBuffers(
					m_swapChainDesc.BufferCount,
					width,
					height,
					m_swapChainDesc.BufferDesc.Format,
					m_swapChainDesc.Flags);

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		HRESULT hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
		if( FAILED( hr ) )
		{
			RERROR_DX11("GetBuffer", hr);
			return false;
		}

		hr = m_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
		pBackBuffer->Release();
		if( FAILED( hr ) )
		{
			RERROR_DX11("CreateRenderTargetView", hr);
			return false;
		}

		SAFE_RELEASE(pBackBuffer);

		m_depthBufferDesc.Width = width;
		m_depthBufferDesc.Height = height;

		if(FAILED(hr = m_pDevice->CreateTexture2D(&m_depthBufferDesc, NULL, &m_pDepthStencil )))
		{
			RERROR_DX11("CreateTexture2D", hr);
			return false;
		}
		ROK("DepthStencilTexture erstellt");

		if(FAILED(m_pDevice->CreateDepthStencilView(m_pDepthStencil, &m_descDSV, &m_pDSV )))
		{
			RERROR_DX11("CreateDepthStencilView", hr);
			return false;
		}
		// bind the render target
		m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, m_pDSV );

		ROK("DepthStencilView erstellt und gebunden");

		// Setup the viewport
		m_vp.Width = (FLOAT)width;
		m_vp.Height = (FLOAT)height;
		m_vp.MinDepth = 0.0f;
		m_vp.MaxDepth = 1.0f;
		m_vp.TopLeftX = 0;
		m_vp.TopLeftY = 0;
		m_pImmediateContext->RSSetViewports( 1, &m_vp );

		m_standby = false;

		return true;
	}
	void raDirectX::CreateScreenShot(const  raString SaveFile)
	{
		m_standby = true;
		static int m_Frame = 0;
		m_Frame++;

		char buf[260];
		sprintf(buf, "%s_%d.png", SaveFile, m_Frame);

		ID3D11Texture2D* pBackBuffer;
		HRESULT hr = m_pSwapChain->GetBuffer( 0, __uuidof( *pBackBuffer ), ( LPVOID* )&pBackBuffer );
		if (hr != S_OK)
			return;

		D3D11_TEXTURE2D_DESC dsc;
		pBackBuffer->GetDesc(&dsc);
		D3D11_RESOURCE_DIMENSION dim;
		pBackBuffer->GetType(&dim);

		ID3D11Texture2D *pCompatableTexture = pBackBuffer;
		if ( dsc.SampleDesc.Count > 1)
		{
			D3D11_TEXTURE2D_DESC dsc_new = dsc;
			dsc_new.SampleDesc.Count = 1;
			dsc_new.SampleDesc.Quality = 0;
			dsc_new.Usage = D3D11_USAGE_DEFAULT;
			dsc_new.BindFlags = 0;
			dsc_new.CPUAccessFlags = 0;
			ID3D11Texture2D *resolveTexture;
			hr = m_pDevice->CreateTexture2D(&dsc_new, NULL, &resolveTexture);
			if ( SUCCEEDED(hr) )
			{
				this->m_pImmediateContext->ResolveSubresource(resolveTexture, 0, pBackBuffer, 0, dsc.Format);
				pCompatableTexture = resolveTexture;
			}
			pCompatableTexture->GetDesc(&dsc);
		}

		hr = D3DX11SaveTextureToFile(this->m_pImmediateContext, pCompatableTexture, D3DX11_IFF_PNG, buf);

		SAFE_RELEASE(pBackBuffer);
		SAFE_RELEASE(pCompatableTexture);

		m_standby = false;
	}
	long raDirectX::OnKeyUp(HWND wnd, WPARAM wParam, LPARAM lParam)
	{
		switch(wParam)
		{
		case RKEY_F11:
			_mkdir("raScreenShots");
			this->CreateScreenShot("raScreenShots\\raShot_");
			break;
		case RKEY_F10:
			this->ChangeMode();
			break;
		};
		return 0;
	}
	long raDirectX::OnKeyDown(HWND wnd, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
	long raDirectX::OnClose(HWND wnd, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
	long raDirectX::OnSize(HWND wnd, WPARAM wParam, LPARAM lParam)
	{
		if( SIZE_MINIMIZED == wParam )
		{
			m_standby = true;
		}
		else if( SIZE_MAXIMIZED == wParam )
		{
			m_standby = false;
			this->Resize();
		}
		else if( SIZE_RESTORED == wParam )
		{
			m_standby = false;
			this->Resize();
		}
		return 0;
	}
	long raDirectX::OnPaint(HWND wnd, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
};