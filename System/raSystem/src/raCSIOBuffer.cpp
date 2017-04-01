#include "..\include\raMain.h"

namespace System
{
	raCSIOBuffer::raCSIOBuffer(raDirectX *dx, LPCSTR ShaderFileName, LPCSTR FunctionName,
			const raFloat* pData, UINT numElements) : raCSRessource(dx, ShaderFileName, FunctionName)
	{
		m_pData = pData;
		m_numElements = numElements;
	}

	raCSIOBuffer::~raCSIOBuffer(void)
	{
	}
	bool raCSIOBuffer::Create()
	{
		raComputeShaderObject::Create();

		D3D11_BUFFER_DESC bufDesc;
		memset(&bufDesc, 0, sizeof(bufDesc));
		bufDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE |
			D3D11_BIND_VERTEX_BUFFER;
		bufDesc.ByteWidth = 4 * m_numElements;
		bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem  = (float*)m_pData;
		HRESULT hr = m_dx->GetDevice()->CreateBuffer(&bufDesc, &InitData, (ID3D11Buffer**)&m_pResource);
		if(FAILED(hr))
		{
			RERROR_DX11("CreateBuffer", hr);
			return false;
		}

		D3D11_UNORDERED_ACCESS_VIEW_DESC viewDesc;
		memset(&viewDesc, 0, sizeof(viewDesc));
		viewDesc.ViewDimension		= D3D11_UAV_DIMENSION_BUFFER;
		viewDesc.Buffer.FirstElement = 0;

		viewDesc.Format				= DXGI_FORMAT_R32_TYPELESS;
		viewDesc.Buffer.Flags		= D3D11_BUFFER_UAV_FLAG_RAW;
		viewDesc.Buffer.NumElements	= bufDesc.ByteWidth / 4;

		hr = m_dx->GetDevice()->CreateUnorderedAccessView(m_pResource, &viewDesc,
			(ID3D11UnorderedAccessView**)&m_pView);

		if(FAILED(hr))
		{
			RERROR_DX11("CreateUnorderedAccessView", hr);
			return false;
		}

		return true;
	}
	bool raCSIOBuffer::RunCS(UINT ThreadGroupCountX,
						   UINT ThreadGroupCountY,
						   UINT ThreadGroupCountZ)
	{
		ID3D11UnorderedAccessView *aView[1] = { (ID3D11UnorderedAccessView*)m_pView };

		m_dx->GetImmediateContext()->CSSetUnorderedAccessViews(0, 1, aView, (UINT*)(&aView));

		raCSRessource::RunCS(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);

		ID3D11UnorderedAccessView* ppViewNull[1] = { 0 };

		m_dx->GetImmediateContext()->CSSetUnorderedAccessViews(0, 1, ppViewNull, (UINT*)(&aView));

		return true;
	}
};