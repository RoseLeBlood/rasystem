#pragma once

	class RAPI raComputeShaderObject
	{
	protected:
		raComputeShaderObject(const GUID& raguid, raDirectX *dx, LPCSTR ShaderFileName, LPCSTR FunctionName);
	public:
		raComputeShaderObject(raDirectX *dx, LPCSTR ShaderFileName, LPCSTR FunctionName);

		virtual bool Create();
		virtual void Destroy();

		virtual bool RunCS(UINT ThreadGroupCountX,
						   UINT ThreadGroupCountY,
						   UINT ThreadGroupCountZ);
	protected:
		raDirectX*			m_dx;
		LPCSTR				m_ShaderFileName;
		LPCSTR				m_FunctionName;

		ID3D11ComputeShader* m_pCS;
	};