#include "..\include\raMain.h"

namespace System
{
	raRTTMaterial::raRTTMaterial(raSmartPointer<raDirectX> dx, LPCSTR effectFile,
								 LPCSTR technique,
								 D3D11_SRV_DIMENSION viewDimension,
								 int envMapWidth,
								 int enyMapHeight,
								 int mipLevels,
								 DXGI_FORMAT fmt,
								 LPCSTR effectVariableName,
								 UINT arraySize) : raTexturedMaterial(dx, effectFile, technique, "RTT",
																	  effectVariableName),
												   m_renderScreen(dx, envMapWidth, enyMapHeight, mipLevels,
																  fmt, arraySize)
	{
		m_ViewDimension = viewDimension;
	}

	bool raRTTMaterial::Create()
	{
		if(!raMaterial::Create())
		{
			RERROR("Beim Erstellen von raMaterial ");
			return false;
		}
		m_pRenderTexture = m_renderScreen.CreateRenderTarget();
		if(!m_pRenderTexture)
		{
			RERROR_OUTOFMEM();
			return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.Format = m_renderScreen.GetFormat();
		desc.ViewDimension = m_ViewDimension;

		UINT arraySize = m_renderScreen.GetArraySize();
		if(arraySize > 1)
		{
			desc.Texture2DArray.FirstArraySlice = 0;
			desc.Texture2DArray.ArraySize = arraySize;
			desc.Texture2DArray.MipLevels = m_renderScreen.GetMipLevels();
			desc.Texture2DArray.MostDetailedMip = 0;
		}
		else
		{
			desc.Texture2D.MipLevels = m_renderScreen.GetMipLevels();
			desc.Texture2D.MostDetailedMip = 0;
		}
		HRESULT hr = m_dx->GetDevice()->CreateShaderResourceView(m_pRenderTexture, &desc, &m_pRV);
		if(FAILED(hr))
		{
			RERROR_DX11("CreateShaderResourceView", hr);
			return false;
		}

		m_ptxDiffuseVariable = m_pEffect->GetVariableByName(m_effectVariableName)->AsShaderResource();

		ROK("raRenderTexturedMaterial erstellt");

		return true;
	}
	bool raRTTMaterial::Destroy()
	{
		raTexturedMaterial::Destroy();

		if(m_pRenderTexture) m_pRenderTexture->Release();
		m_renderScreen.Destroy();

		return true;
	}

	void raRTTMaterial::GetSizeFromBackBuffer(const DXGI_SURFACE_DESC* desc)
	{
	}
};