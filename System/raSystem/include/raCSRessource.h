#pragma once

// Compute Shader Object mit Ressourcen

	class RAPI raCSRessource : public raComputeShaderObject
	{
	public:
		raCSRessource(raDirectX *dx, LPCSTR ShaderFileName, LPCSTR FunctionName);
		virtual ~raCSRessource(void);

	protected:
		ID3D11Resource		*m_pResource;
		ID3D11View			*m_pView;
	};

	class RAPI raCSRessourceTexture1D : public raCSRessource
	{
	public:
		raCSRessourceTexture1D(raDirectX *dx, LPCSTR ShaderFileName, LPCSTR FunctionName,
			const raFloat* pData, UINT numElements);

		virtual bool Create();
		virtual void Destroy();

		virtual bool RunCS(UINT ThreadGroupCountX,
						   UINT ThreadGroupCountY,
						   UINT ThreadGroupCountZ);
	protected:
		const raFloat* m_pData;
		UINT		   m_numElements;
	};