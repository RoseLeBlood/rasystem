#include "..\include\raMain.h"

namespace System
{
	raRenderScreen::raRenderScreen(raSmartPointer<raDirectX> dx, int width, int height, int mipLevels,
			DXGI_FORMAT fmt, UINT arraySize)
	{
		m_dx = dx;
		m_Width = width;
		m_Height = height;
		m_Format = fmt;

		m_pRTView = NULL;
		m_pDSView = NULL;
		m_pRenderTarget = NULL;
		m_pDepth = NULL;
		m_MipLevels = mipLevels;

		m_arraySize = arraySize;
	}
	bool raRenderScreen::CreateDepthTexture(D3D11_TEXTURE2D_DESC &desc)
	{
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.Format = DXGI_FORMAT_D32_FLOAT;

		HRESULT hr = m_dx->GetDevice()->CreateTexture2D( &desc, NULL, &m_pDepth);
		if(FAILED(hr))
		{
			RERROR_DX11("CreateTexture2D", hr);
			return false;
		}

		return true;
	}
	bool raRenderScreen::CreateTexture(D3D11_TEXTURE2D_DESC &desc)
	{
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.Format = m_Format;

		HRESULT hr = m_dx->GetDevice()->CreateTexture2D(&desc, NULL, &m_pRenderTarget);
		if(FAILED(hr))
		{
			RERROR_DX11("CreateTexture2D", hr);
			return false;
		}

		return true;
	}
	bool raRenderScreen::CreateDepthStencilView()
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc) );
		desc.Format = DXGI_FORMAT_D32_FLOAT;

		if(m_arraySize > 1)
		{
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			desc.Texture2DArray.FirstArraySlice = 0;
			desc.Texture2DArray.ArraySize = m_arraySize;
			desc.Texture2DArray.MipSlice = 0;
		}
		else
		{
			desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		}

		HRESULT hr = m_dx->GetDevice()->CreateDepthStencilView(m_pDepth, &desc, &m_pDSView);
		if(FAILED(hr))
		{
			RERROR_DX11("CreateDepthStencilView", hr);
			return false;
		}

		return true;
	}
	bool raRenderScreen::CreateRenderTargetView()
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.Format = m_Format;

		if(m_arraySize > 1)
		{
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			desc.Texture2DArray.FirstArraySlice = 0;
			desc.Texture2DArray.ArraySize = m_arraySize;
			desc.Texture2DArray.MipSlice = 0;
		}
		else
		{
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
		}
		HRESULT hr = m_dx->GetDevice()->CreateRenderTargetView(m_pRenderTarget, &desc, &m_pRTView);
		if(FAILED(hr))
		{
			RERROR_DX11("CreateRenderTargetView", hr);
			return false;
		}
		return true;
	}
	ID3D11Texture2D* raRenderScreen::CreateRenderTarget()
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = m_Width;
		desc.Height = m_Height;
		desc.ArraySize = m_arraySize;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.CPUAccessFlags = 0;
		if(m_arraySize == 6)
			desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

		if(!CreateDepthTexture(desc))
		{
			RERROR("CreateDepthTexture");
			return NULL;
		}

		desc.MipLevels = m_MipLevels;
		if(m_MipLevels > 1) //keine Mips für DepthTexture
			desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		if(!CreateTexture(desc))
		{
			RERROR("CreateTexture");
			return NULL;
		}
		if(!CreateDepthStencilView())
		{
			RERROR("CreateDepthStencilView");
			return NULL;
		}
		if(!CreateRenderTargetView())
		{
			RERROR("CreateRenderTargetView");
			return NULL;
		}
		m_pRenderTarget->AddRef();

		return m_pRenderTarget;
	}
	void raRenderScreen::Destroy()
	{
		SAFE_RELEASE(m_pRTView);
		SAFE_RELEASE(m_pDSView);
		SAFE_RELEASE(m_pRenderTarget);
		SAFE_RELEASE(m_pDepth);
	}
	raDirectX* raRenderScreen::GetDirectX()
	{
		return m_dx.get();
	}
	ID3D11Texture2D* raRenderScreen::GetTexture()
	{
		m_pRenderTarget->AddRef();
		return m_pRenderTarget;
	}
	DXGI_FORMAT raRenderScreen::GetFormat()
	{
		return m_Format;
	}
	int raRenderScreen::GetMipLevels()
	{
		return m_MipLevels;
	}
	UINT raRenderScreen::GetArraySize()
	{
		return m_arraySize;
	}
};