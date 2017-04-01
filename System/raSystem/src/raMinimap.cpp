#include "..\include\raMain.h"

namespace System
{
	raMinimap::raMinimap(raSmartPointer<raDirectX> dx, LPCSTR playerIconFile, bool bAlpha):
						raTexturedQuad(dx, new raRTTMaterial(dx.get(),
												"raEffects\\Overlay.fx",
												"RenderWithTextureOverlay",
												D3D11_SRV_DIMENSION_TEXTURE2D, 640, 480),
									   bAlpha), m_PlayerIcon(m_PlayerIcon)
	{
		m_pRVPlayerIconTexture = NULL;
		m_ptxPlayerIconTextureVariable = NULL;
	}

	void raMinimap::CreateEffectVariables()
	{
		ID3DX11Effect* pEffect = GetMaterial(0)->GetEffect();

		raTexturedQuad::CreateEffectVariables();

		//Normalerweise wohl nur ein Material
		//Sonst müsste man in SetupEffectVariables alle Materialien durchlaufen
		m_ptxPlayerIconTextureVariable = pEffect->
			GetVariableByName("g_PlayerIconTexture" )->AsShaderResource();

		raResource::Instance().
			CreateTextureFromFile(m_dx.get(),
			m_PlayerIcon,
			&m_pRVPlayerIconTexture);
	}

	void raMinimap::SetupEffectVariables(const raMatrix& pView,
		const raMatrix& pProj, const raLight* pLight)
	{
		raTexturedQuad::SetupEffectVariables(pView, pProj, pLight);

		//m_ptxPlayerIconTextureVariable kann evtl. auch schon in CreateEffectVariables
		//gesetzt werden, wenn sich die Textur nicht ändert.
		if(m_ptxPlayerIconTextureVariable &&
			!IsErrorResource(m_pRVPlayerIconTexture))
			m_ptxPlayerIconTextureVariable->SetResource(
			m_pRVPlayerIconTexture);
	}

	void raMinimap::Destroy()
	{
		raTexturedQuad::Destroy();
		SAFE_RELEASE(m_pRVPlayerIconTexture);
	}
};