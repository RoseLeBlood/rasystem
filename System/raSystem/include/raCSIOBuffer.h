#pragma once

	class RAPI raCSIOBuffer : public raCSRessource
	{
	public:
		raCSIOBuffer(raDirectX *dx, LPCSTR ShaderFileName, LPCSTR FunctionName,
			const raFloat* pData, UINT numElements);
		virtual ~raCSIOBuffer(void);

		virtual bool Create();

		virtual bool RunCS(UINT ThreadGroupCountX,
						   UINT ThreadGroupCountY,
						   UINT ThreadGroupCountZ);

		ID3D11Buffer*	GetBuffer()				{ return (ID3D11Buffer*)m_pResource; }

	protected:
		const raFloat*		m_pData;
		UINT				m_numElements;
	};