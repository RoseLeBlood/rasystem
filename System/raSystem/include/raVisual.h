#pragma once

	class RAPI raVisual : public raRender, public IRAMesh
	{
	public:
		static raString	EffectfileName;
		static raString	TechniqueName;
		static raString	TexturedTechniqueName;

		static raString	SkinnedEffectfileName;
		static raString	SkinnedEffectfileName2;
		static raString	SkinnedTechniqueName;
		static raString	SkinnedTexturedTechniqueName;

	public:
		raVisual( raSmartPointer<raDirectX> dx, bool bAlpha = false, bool bAdditive = false,
				 bool bDepthDisable = false, D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID,
				 D3D11_CULL_MODE cullMode = D3D11_CULL_BACK);
		~raVisual(void);

		virtual bool CreateVertexLayout();
		virtual bool Create();
		virtual void Destroy();

		virtual void CreateEffectVariables() {};
		virtual void DestroyEffectVariables() {};

		virtual void SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight);
		virtual bool Render(UINT drawOrder, const raMatrix &pView, const raMatrix &pProj,
							const raLight* pLight, LPCSTR techniqueName = "");

		virtual void CreateMaterials();
		virtual void DestroyMaterials();

		bool SetState();
		bool RestoreState();

		virtual const D3D11_INPUT_ELEMENT_DESC* GetVertexLayout()
		{
			return VERTEXPOSITIONNORMALTEXTURED::GetVertexLayout();
		}

		virtual UINT GetNumElements()
		{
			return VERTEXPOSITIONNORMALTEXTURED::GetNumElements();
		}

		virtual UINT GetStrideSize()
		{
			return sizeof(VERTEXPOSITIONNORMALTEXTURED);
		}

		virtual DXGI_FORMAT GetIndexBufferFormat()
		{
			return DXGI_FORMAT_R16_UINT;
		}
		// BoundingBox
		raVector3				m_bbMin;
		raVector3				m_bbMax;

		virtual UINT GetNumMaterials()				{ return m_numMaterials; }
		virtual raMaterial *GetMaterial(UINT n)		{ return m_pMaterials[n]; }

		// Tauscht ein Material nur aus!!
		void SetMaterial(UINT n, raMaterial* pMaterial);
		// erst nach Create aufrufen ( Auswechesln eines nicht gemeinsam genutztes Material )
		void SetMaterial_1(UINT n, raMaterial* pMaterial);
		void SetTexture(UINT n, LPCSTR textureFileName);
		void SetTexture_1(UINT n, LPCSTR textureFileName);

		virtual void SetVisible(bool bVisible);
		virtual void SetDrawOder(UINT Value)	{ if(Value > RAMAX_DRAWODER) Value = RAMAX_DRAWODER; m_DrawOder = Value; }
		virtual void SetSelected(bool bSelected) {};

		virtual const float GetBoundingsphereRadius()
		{
			if((float)m_bbMax.x > -FLT_MAX)
			{
				raVector3 center = 0.5f * (m_bbMin + m_bbMax);
				raVector3 v = m_bbMax - center;
				float r = RAMAX(m_World.m41 * (float)v.x,
					RAMAX(m_World.m42 * (float)v.y, m_World.m43 * (float)v.z));
				return r;
			}
			else
				return 1;
		}
	protected:
		bool CreateAlphaBlendState();
		bool CreateDepthStencilState();
		bool CreateRasterizerState();

	protected:
		UINT					 m_DrawOder;
		DWORD					 m_numMaterials;
		raMaterial				 **m_pMaterials;

		raSmartPointer<raDirectX>				 m_dx;
		ID3D11InputLayout*		 m_pVertexLayout;
		ID3D11BlendState*		 m_pAlphaBlendState;
		ID3D11BlendState*		 m_pOldAlphaBlendState;
		ID3D11DepthStencilState* m_pDepthStencilState;
		ID3D11DepthStencilState* m_pOldDepthStencilState;
		ID3D11RasterizerState*	 m_pRasterState;
		ID3D11RasterizerState*	 m_pOldRasterState;

		bool					 m_bVisible;
		bool					 m_bAlpha;
		bool					 m_bAdditive;
		float					 m_blendFactor[4];
		UINT					 m_sampleMask;

		bool					 m_bDepthDisable;
		UINT					 m_StencilRef;

		D3D11_FILL_MODE			 m_FillMode;
		D3D11_CULL_MODE			 m_CullMode;

		DWORD					m_numSubsets;
		SDKMESH_SUBSET*			m_pSubsets;
	};