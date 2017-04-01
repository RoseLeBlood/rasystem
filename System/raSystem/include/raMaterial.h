#pragma once

	typedef std::map<raString, ID3DX11Effect*> raEffectMap;

	class RAPI raMaterial  // MATERIAL
	{
	public:
		raMaterial(raSmartPointer<raDirectX> dx, raString EffectFile, raString Technique);
		~raMaterial(void);

		virtual bool Create();
		virtual bool Destroy();
		virtual bool CreateVertexLayout(const D3D11_INPUT_ELEMENT_DESC* pLayout,
			UINT nemElements, ID3D11InputLayout** ppInputLayout);

		virtual void Setup();
		virtual void SetupPerFrame(const raLight* Light, const raMatrix* pView = NULL,
								   const raMatrix* pProj = NULL);
		virtual void SetupPerObject(const raMatrix& mWorld, const raMatrix& mWorldView,
									const raMatrix& mWorldViewProjection);

		void SetAmbientColor(raColor Ambient)				{ m_AmbientColor = Ambient; }
		void SetDiffuseColor(raColor Diffuse)				{ m_DiffuseColor = Diffuse; }
		void SetTechnique(LPCSTR TechniqueName)
		{
			m_TechniqueName = TechniqueName;
			m_pTechnique = m_pEffect->GetTechniqueByName(TechniqueName);
		}

		ID3DX11Effect*			GetEffect(void)				{ return m_pEffect; }
		ID3DX11EffectTechnique*	GetEffectTechnique(LPCSTR techniqueName = "")
		{
			if(!strcmp("", techniqueName))
				return m_pTechnique;
			else
				return m_pEffect->GetTechniqueByName(techniqueName);
		}
		raString				GetTechniqueName(void)		{ return m_TechniqueName; }
		raString				GetEffectFileName(void)		{ return m_EffectFileName; }

		void SelectTechnique(raString techniqueName)
		{
			m_TechniqueName = techniqueName;
			m_pTechnique    = m_pEffect->
				GetTechniqueByName(techniqueName.c_str());
		}

		static raEffectMap				m_effects;
		bool							m_bSelected;
	protected:
		void CreateEffectFromCompiledFile(raString strFilename, DWORD dwShaderFlags);
	protected:
		raSmartPointer<raDirectX>		m_dx;
		raString						m_EffectFileName;
		raString						m_TechniqueName;

		ID3DX11Effect*					m_pEffect;
		ID3DX11EffectTechnique*			m_pTechnique;
		ID3DX11EffectVectorVariable*	m_pMaterialDiffuseColorVariable;
		ID3DX11EffectVectorVariable*	m_pMaterialAmbientVariable;
		ID3DX11EffectScalarVariable*	m_pSelectedVariable;

		raColor							m_DiffuseColor;
		raColor							m_AmbientColor;
	};