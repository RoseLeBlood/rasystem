#include "..\include\raMain.h"

namespace System
{
	raString raVisual::EffectfileName("raEffects\\MyEngine.fx");
	raString raVisual::TechniqueName("RenderWithMaterialColor");
	raString raVisual::TexturedTechniqueName("RenderScene");
	raString raVisual::SkinnedEffectfileName("raEffects\\Animation.fx");
	raString raVisual::SkinnedEffectfileName2("raEffects\\AnimationFromTex.fx");
	raString raVisual::SkinnedTechniqueName("RenderAnimationNoTexSelectable");
	raString raVisual::SkinnedTexturedTechniqueName("RenderAnimationSelectable");

	raVisual::raVisual(raSmartPointer<raDirectX> dx, bool bAlpha, bool bAdditive,
					   bool bDepthDisable, D3D11_FILL_MODE fillMode,
					   D3D11_CULL_MODE cullMode) : raRender(dx),
					   m_bbMin(-FLT_MAX, -FLT_MAX, -FLT_MAX),
					   m_bbMax(FLT_MAX, FLT_MAX, FLT_MAX)
	{
		m_dx = dx;
		m_DrawOder = 1;
		m_numMaterials = 0;
		m_pMaterials = NULL;
		m_bAlpha = bAlpha;
		m_bAdditive = bAdditive;
		m_bDepthDisable = bDepthDisable;
		m_sampleMask = 0xffffffff;
		m_pAlphaBlendState = NULL;
		m_pOldAlphaBlendState = NULL;
		m_pDepthStencilState = NULL;
		m_pOldDepthStencilState = NULL;
		m_pRasterState = NULL;
		m_pOldRasterState  = NULL;
		m_FillMode = fillMode;
		m_CullMode = cullMode;
		m_bVisible = true;
	}
	raVisual::~raVisual()
	{
		/*for(DWORD i = 0; i < m_numMaterials; i++)
		{
			SAFE_DELETE(m_pMaterials[i]);
		}
		SAFE_DELETE_ARRAY(m_pMaterials);*/
	}
	bool raVisual::Create()
	{
		bool result = true;

		if(!CreateMesh())
		{
			RERROR("Beim Erstellen des Meshs");
			return false;
		}
		CreateMaterials();
		CreateEffectVariables();

		m_pAlphaBlendState = NULL;
		if(m_bAlpha) result = CreateAlphaBlendState();
		if(!result)
		{
			RERROR("Beim Erstellen des AlphaBlendState");
			return false;
		}

		m_pDepthStencilState = NULL;
		if(m_bDepthDisable) result =  CreateDepthStencilState();
		if(!result)
		{
			RERROR("Beim Erstellen des DepthStencilState");
			return false;
		}

		m_pRasterState = NULL;
		if(m_FillMode != D3D11_FILL_SOLID ||
		   m_CullMode != D3D11_CULL_BACK)
			result =  CreateRasterizerState();
		if(!result)
		{
			RERROR("Beim Erstellen des RasterizerState");
			return false;
		}

		result =  CreateVertexLayout();
		if(!result)
		{
			RERROR("Beim Erstellen des VertexLayout");
			return false;
		}

		ROK("raVisual erstellt");
		return true;
	}
	void raVisual::CreateMaterials()
	{
		for(DWORD matNo = 0; matNo < m_numMaterials; matNo++)
		{
			 if(m_pMaterials[matNo]) m_pMaterials[matNo]->Create();
		}
	}
	void raVisual::Destroy()
	{
		/*DestroyMaterials();
		DestroyEffectVariables();
		DestroyMesh();
		SAFE_RELEASE(m_pRasterState);
		SAFE_RELEASE(m_pVertexLayout);
		SAFE_RELEASE(m_pDepthStencilState);
		SAFE_RELEASE(m_pAlphaBlendState);*/
	}
	void raVisual::DestroyMaterials()
	{
		for(DWORD matNo = 0; matNo < m_numMaterials; matNo++)
		{
			if(m_pMaterials[matNo]) m_pMaterials[matNo]->Destroy();
		}
	}
	void raVisual::SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										const raLight* pLight)
	{
		raMatrix mWorldView, mWorldViewProjection;

		mWorldView = m_World * pView;
		mWorldViewProjection = mWorldView * pProj;

		raMatrix mLightView, mLightProj;
		mLightView = pLight->GetViewMatrix();
		mLightProj = pLight->GetProjectionMatrix();

		raMatrix mLightWorldView = m_World * mLightView;
		raMatrix mLightWorldViewProjection = mLightWorldView * mLightProj;

		for(DWORD i=0;i < m_numMaterials; i++)
		{
			m_pMaterials[i]->SetupPerObject(m_World, mWorldView, mWorldViewProjection);

			ID3DX11EffectMatrixVariable* pmLightViewProj =
				m_pMaterials[i]->GetEffect()->GetVariableByName("g_mLightViewProjection")->
				AsMatrix();
			if(pmLightViewProj) pmLightViewProj->SetMatrix((float*)&mLightWorldViewProjection);
		}
	}
	bool raVisual::Render(UINT drawOrder, const raMatrix &pView, const raMatrix &pProj,
							const raLight* pLight, LPCSTR techniqueName)
	{
		if(!m_bVisible || m_DrawOder != drawOrder) return true;

		SetState();
		SetupEffectVariables(pView, pProj, pLight);

		m_dx->GetImmediateContext()->IASetInputLayout(m_pVertexLayout);

		RenderMesh(techniqueName);
		RestoreState();
		return true;
	}
	bool raVisual::CreateVertexLayout()
	{
		if(m_pMaterials && m_pMaterials[0])
		{
			if(!m_pMaterials[0]->CreateVertexLayout(GetVertexLayout(), GetNumElements(), &m_pVertexLayout))
				return false;
		}
		return true;
	}
	bool raVisual::SetState()
	{
		if(m_FillMode != D3D11_FILL_SOLID || m_CullMode != D3D11_CULL_BACK)
		{
			m_dx->GetImmediateContext()->RSGetState(&m_pOldRasterState);
			m_dx->GetImmediateContext()->RSSetState(m_pRasterState);
		}

		if(m_bAlpha)
		{
			m_dx->GetImmediateContext()->OMGetBlendState(&m_pOldAlphaBlendState, m_blendFactor, &m_sampleMask);
			m_dx->GetImmediateContext()->OMSetBlendState(m_pAlphaBlendState, NULL, 0xffffffff);
		}

		if(m_bDepthDisable)
		{
			m_dx->GetImmediateContext()->OMGetDepthStencilState(&m_pOldDepthStencilState, &m_StencilRef);
			m_dx->GetImmediateContext()->OMSetDepthStencilState(m_pDepthStencilState, 0);
		}
		return true;
	}
	bool raVisual::RestoreState()
	{
		if(m_FillMode != D3D11_FILL_SOLID || m_CullMode != D3D11_CULL_BACK)
		{
			m_dx->GetImmediateContext()->RSSetState(m_pOldRasterState);
			SAFE_RELEASE(m_pOldRasterState);
		}

		if(m_bAlpha)
		{
			m_dx->GetImmediateContext()->OMSetBlendState(m_pOldAlphaBlendState, m_blendFactor, m_sampleMask);
			SAFE_RELEASE(m_pOldAlphaBlendState);
		}

		if(m_bDepthDisable)
		{
			m_dx->GetImmediateContext()->OMSetDepthStencilState(m_pOldDepthStencilState, m_StencilRef);
			SAFE_RELEASE(m_pOldDepthStencilState);
		}
		return true;
	}
	bool raVisual::CreateAlphaBlendState()
	{
		D3D11_BLEND_DESC desc;
		memset(&desc, 0, sizeof(D3D11_BLEND_DESC));
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		if(m_bAdditive)
		{
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		}
		else
		{
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		}
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

		//alpha dar Destination bleibt erhalten
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		HRESULT hr = m_dx->GetDevice()->CreateBlendState(&desc, &m_pAlphaBlendState);
		if(FAILED(hr))
		{
			RERROR_DX11("CreateBlendState", hr);
			return false;
		}
		return true;
	}
	bool raVisual::CreateDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		memset(&desc, 0 , sizeof(D3D11_DEPTH_STENCIL_DESC));
		desc.DepthEnable = true;
		desc.DepthFunc = D3D11_COMPARISON_LESS;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.StencilEnable = false;

		HRESULT hr = m_dx->GetDevice()->CreateDepthStencilState(&desc, &m_pDepthStencilState);
		if(FAILED(hr))
		{
			RERROR_DX11("CreateDepthStencilState", hr);
			return false;
		}

		return true;
	}
	bool raVisual::CreateRasterizerState()
	{
		D3D11_RASTERIZER_DESC desc;

		desc.FillMode = m_FillMode;
		desc.CullMode = m_CullMode;
		desc.FrontCounterClockwise = true;
		desc.DepthBias = false;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = false;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = m_dx->GetDevice()->CreateRasterizerState(&desc, &m_pRasterState);
		if(FAILED(hr))
		{
			RERROR_DX11("CreateRasterizerState", hr);
			return false;
		}

		return true;
	}
	// Tauscht ein Material nur aus!!
	void raVisual::SetMaterial(UINT n, raMaterial* pMaterial)
	{
		m_pMaterials[n] = pMaterial;

		if(n == 0)
		{
			SAFE_RELEASE(m_pVertexLayout);
			CreateVertexLayout();
		}
	}
	void raVisual::SetMaterial_1(UINT n, raMaterial* pMaterial)
	{
		if(m_pMaterials[n])
		{
			m_pMaterials[n]->Destroy();
			delete m_pMaterials[n];
			SetMaterial(n, m_pMaterials[n]);
			m_pMaterials[n]->Create();
		}
	}
	void raVisual::SetTexture(UINT n, LPCSTR textureFileName)
	{
		raTexturedMaterial* pMaterial = (raTexturedMaterial*)m_pMaterials[n];
		if(pMaterial) pMaterial->SetTexture(textureFileName);
	}
	void raVisual::SetTexture_1(UINT n, LPCSTR textureFillName)
	{
		raTexturedMaterial* pMaterial = (raTexturedMaterial*)m_pMaterials[n];
		if(pMaterial)
		{
			pMaterial->Destroy();
			pMaterial->SetTexture(textureFillName);
			pMaterial->Create();
		}
	}
	void raVisual::SetVisible(bool bVisible)
	{
		m_bVisible = bVisible;
	}
};