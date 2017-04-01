#include "..\include\raMain.h"

namespace System
{
	raComputeShaderObject::raComputeShaderObject(raDirectX *dx,
		LPCSTR ShaderFileName, LPCSTR FunctionName) : m_ShaderFileName(ShaderFileName),
													  m_FunctionName(FunctionName)
	{
		m_dx = dx;
		m_pCS = NULL;
	}
	bool raComputeShaderObject::Create()
	{
		ID3DBlob* pCSBuf = NULL;
		DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;

		HRESULT hr = D3DX11CompileFromFile(m_ShaderFileName, NULL, NULL, m_FunctionName, "cs_5_0",
			dwShaderFlags, 0, NULL, &pCSBuf, NULL, NULL);

		if(FAILED(hr))
		{
			RERROR_DX11("D3DX11CompileFromFile", hr);
			return false;
		}

		hr = m_dx->GetDevice()->CreateComputeShader((DWORD*)pCSBuf->GetBufferPointer(),
			pCSBuf->GetBufferSize(), NULL, &m_pCS);

		if(FAILED(hr))
		{
			RERROR_DX11("CreateComputeShader", hr);
			return false;
		}

		SAFE_RELEASE(pCSBuf);

		return true;
	}
	void raComputeShaderObject::Destroy()
	{
		SAFE_RELEASE(m_pCS);
	}
	bool raComputeShaderObject::RunCS(UINT ThreadGroupCountX, UINT ThreadGroupCountY,
						              UINT ThreadGroupCountZ)
	{
		m_dx->GetImmediateContext()->CSSetShader(m_pCS, NULL, 0);
		m_dx->GetImmediateContext()->Dispatch(ThreadGroupCountX, ThreadGroupCountY,
			ThreadGroupCountZ);

		return true;
	}
};