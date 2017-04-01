#pragma once

	class RAPI raGSEntity : public raEntity<VERTEXPOSITIONTEXTURED>
	{
	public:
		raGSEntity(raSmartPointer<raDirectX> dx, raSmartPointer<raMaterial> pMaterial  = raSmartPointer<raMaterial>(NULL));

		virtual void SetupVertices();
		virtual void SetupIndices();

		virtual bool Create();
		virtual void Destroy();
		virtual bool RenderMesh(LPCSTR techniqueName ="");

		void SetSplietRatio(float splitRatio)
		{
			m_SplitRatio = splitRatio;
		}

	private:
		void CreateGeometry();
	private:
		ID3DX11EffectTechnique*			m_pCreateGeometryTechnique;
		ID3D11Buffer*					m_pStreamTo;
		ID3D11Buffer*					m_pDrawFrom;
		ID3DX11EffectScalarVariable*	m_SplitRatioVariable;

		bool							m_bOriginalBufferVerwenden;
		float							m_SplitRatio;
	};