#include "..\include\raMain.h"

namespace System
{
	raCSRessource::raCSRessource(raDirectX *dx, LPCSTR ShaderFileName, LPCSTR FunctionName) :
		raComputeShaderObject(dx, ShaderFileName, FunctionName)
	{
		m_pResource = NULL;
		m_pView = NULL;
	}

	raCSRessource::~raCSRessource(void)
	{
		SAFE_RELEASE(m_pResource);
		SAFE_RELEASE(m_pView);
	}
	//-----------------------------------------
	raCSRessourceTexture1D::raCSRessourceTexture1D(raDirectX *dx,
		LPCSTR ShaderFileName,
		LPCSTR FunctionName,
		const raFloat* pData,
		UINT numElements) : raCSRessource(dx, ShaderFileName, FunctionName)
	{
		m_pData = pData;
		m_numElements = numElements;
	}
	bool raCSRessourceTexture1D::Create()
	{
		raComputeShaderObject::Create();

		D3D11_TEXTURE1D_DESC texDesc;
		texDesc.Width		= m_numElements;
		texDesc.MipLevels	= 1;
		texDesc.Format		= DXGI_FORMAT_R32_FLOAT;
		texDesc.Usage		= D3D11_USAGE_DEFAULT;
		texDesc.BindFlags	= D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags	= 0;
		texDesc.ArraySize	= 1;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem			= (float*)m_pData;
		InitData.SysMemPitch		= sizeof(float);
		InitData.SysMemSlicePitch	= sizeof(float);

		HRESULT hr = m_dx->GetDevice()->CreateTexture1D(&texDesc, &InitData,
			(ID3D11Texture1D**)&m_pResource);

		if(FAILED(hr))
		{
			RERROR_DX11("CreateTexture1D", hr);
			return false;
		}
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		memset(&viewDesc, 0, sizeof(viewDesc));
		viewDesc.Format			= texDesc.Format;
		viewDesc.ViewDimension	= D3D11_SRV_DIMENSION_TEXTURE1D;
		viewDesc.Texture2D.MipLevels = texDesc.MipLevels;

		hr = m_dx->GetDevice()->CreateShaderResourceView(m_pResource, &viewDesc,
			(ID3D11ShaderResourceView **)&m_pView);

		if(FAILED(hr))
		{
			RERROR_DX11("CreateShaderResourceView", hr);
			return false;
		}

		return true;
	}
	void raCSRessourceTexture1D::Destroy()
	{
		SAFE_DELETE(m_pData);
		m_numElements = NULL;
	}
	bool raCSRessourceTexture1D::RunCS(UINT ThreadGroupCountX,
									   UINT ThreadGroupCountY,
									   UINT ThreadGroupCountZ)
	{
		ID3D11ShaderResourceView* aViews[1] = { (ID3D11ShaderResourceView*) m_pView };
		m_dx->GetImmediateContext()->CSSetShaderResources(0, 1, aViews);

		raCSRessource::RunCS(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);

		ID3D11ShaderResourceView* ppViewNULL[1] = { NULL };
		m_dx->GetImmediateContext()->CSSetShaderResources( 0, 1, ppViewNULL );

		return true;
	}
};