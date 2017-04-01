#include "..\include\raMain.h"

namespace System
{
	raSceneManager::raSceneManager(raSmartPointer<raCamera> Camera)
	{
		m_Camera = Camera;

		for(int i = 0; i < numLights; i++) m_Lights[i] = new raLight();

		m_dx       = NULL;
		m_pShadowReceiverMaterial = NULL;
		m_bCreated = false;
	}

	raSceneManager::~raSceneManager(void)
	{
		/*for(UINT i = 0; i < m_Visuals.size(); i++)
		{
			SAFE_DELETE(m_Visuals[i]);
		}
		SAFE_DELETE(m_pShadowReceiverMaterial);

		for(int i = 0; i < numLights; i++)
			SAFE_DELETE(m_Lights[i]);*/

		m_dx = NULL;
	}
	bool raSceneManager::Add(raRender* Object)
	{
		m_Visuals.push_back(Object);
		if(m_bCreated) Object->Create();
		return true;
	}
	bool raSceneManager::Remove(raRender* Object)
	{
		for(UINT i = 0; i < m_Visuals.size(); i++)
		{
			if(m_Visuals[i] == Object)
			RemoveAt(i);
		}
		return false;
	}
	bool raSceneManager::RemoveAt(int i)
	{
		raRender* pObject = m_Visuals[i];
		pObject->Destroy();
		delete pObject;
		m_Visuals.erase(m_Visuals.begin() + i);

		return true;
	}
	bool raSceneManager::Create(raSmartPointer<raDirectX> dx)
	{
		m_dx = dx;

		CreateCubeMapMatrices();

		for(UINT i = 0; i < m_Visuals.size(); i++)
		{
			m_Visuals[i]->Create();
		}

		m_bCreated = true;

		//Gerenderter Schatten
		m_pShadowReceiverMaterial = new raRTTMaterial(dx.get(),
		"raEffects\\LightMapEffect.fx", "RenderWithLightMap",
			D3D11_SRV_DIMENSION_TEXTURE2D, 1024, 1024, 1, DXGI_FORMAT_R32_FLOAT,
			"g_shadowTexture");
		m_pShadowReceiverMaterial->Create();

		return true;
	}
	void raSceneManager::Destroy()
	{
		for(UINT i = 0; i < m_Visuals.size(); i++)
		{
			if(m_Visuals[i]) m_Visuals[i]->Destroy();
		}

		//Materialien aufräumen
		//Die EffectMap hält eine eigene Referenz auf die Effekte, die auch
		//freigegeben werden muss. Diese Referenz ist dafür gut, dass das
		//Effekt-Objekt nicht gelöscht werden kann, wenn es einmal in der
		//Effektmap drin ist - auch dann nicht, wenn es momentan nicht mehr
		//verwendet wird.
		raEffectMap::iterator iterator;
		for(iterator = raMaterial::m_effects.begin();
			iterator != raMaterial::m_effects.end();
			iterator++)
		{
			SAFE_RELEASE(iterator->second);
		}
	}
	void raSceneManager::Update(float fTime, float fRunTime)
	{
		raEffectMap::iterator iterator;
		for(iterator = raMaterial::m_effects.begin();
			iterator != raMaterial::m_effects.end();
			iterator++)
		{
			ID3DX11Effect* pEffect = iterator->second;

			//Für Animationen
			pEffect->GetVariableByName("g_fTime")->AsScalar()->SetFloat((float)fRunTime);
		}
		for(UINT i = 0; i < m_Visuals.size(); i++)
		{
			m_Visuals[i]->Update(fTime, fRunTime);
		}
	}
	void raSceneManager::Render(const raMatrix& pView, const raMatrix &pProj, const raLight* pLight,
								LPCSTR techniqueName)
	{
		SetupEffectVariables(pView, pProj);

		//Alternative: Vorsortieren nach DrawOrder
		for(int drawOrder = 0; drawOrder < 5; drawOrder++)
		{
			for(UINT i = 0; i < m_Visuals.size(); i++)
			{
				m_Visuals[i]->Render(drawOrder, pView, pProj, pLight, techniqueName);
			}
		}
	}
	void raSceneManager::Render()
	{
		if(m_pShadowReceiverMaterial)
			m_pShadowReceiverMaterial->Setup(); //Nicht in SetupEffectVariables, da nicht für Rendern in Lightmap

		Render(m_Camera->GetViewMatrix(),
			m_Camera->GetProjMatrix(),
			&m_Light);
	}
	raRender* raSceneManager::GetItem(int index)
	{
		return m_Visuals[index];
	}
	void raSceneManager::ShowAllObjects(bool show)
	{
		for(UINT i = 0; i < m_Visuals.size(); i++)
		{
			m_Visuals[i]->SetVisible(show);
		}
	}

	void raSceneManager::SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj)
	{
		raVector3 camPos(*m_Camera->GetEyePt());

		raEffectMap::iterator iterator;
		for(iterator = raMaterial::m_effects.begin();
			iterator != raMaterial::m_effects.end();
			iterator++)
		{
			ID3DX11Effect* pEffect = iterator->second;

			SetupLights(pEffect, &m_Light);

			ID3DX11EffectMatrixVariable* pmView = pEffect->GetVariableByName("g_mView")->AsMatrix();
			if(pmView) pmView->SetMatrix((float*)&pView);

			ID3DX11EffectMatrixVariable* pmProj = pEffect->GetVariableByName("g_mProj")->AsMatrix();
			if(pmProj) pmProj->SetMatrix((float*)&pProj);

			ID3DX11EffectVectorVariable* pmCamPos = pEffect->GetVariableByName("g_cameraPos")->AsVector();
			if(pmCamPos) pmView->SetMatrix((float*)&camPos);
		}
	}
	void raSceneManager::SetupLights(ID3DX11Effect* pEffect, const raLight* pLight)
	{
			ID3DX11EffectVectorVariable* pLightPosVariable     =
				pEffect->GetVariableByName("g_LightPos" )->AsVector();
			raVector4 lightPos(pLight->m_Position.x, pLight->m_Position.y, pLight->m_Position.z, 1);
			if(pLightPosVariable)
				pLightPosVariable->SetFloatVector((float*)&lightPos);

			ID3DX11EffectVectorVariable* pLightDirVariable     =
				pEffect->GetVariableByName("g_vLightDir" )->AsVector();
			raVector3 lightDir = pLight->m_Direction;
			if(pLightDirVariable)
				pLightDirVariable->SetFloatVector((float*)&lightDir);

			ID3DX11EffectVectorVariable* pLightDiffuseVariable =
				pEffect->GetVariableByName("g_LightDiffuse" )->AsVector();
			if(pLightDiffuseVariable)
				pLightDiffuseVariable->SetFloatVector((float*)&pLight->m_DiffuseColor);
			ID3DX11EffectVectorVariable* pAmbientVariable      =
				pEffect->GetVariableByName("g_LightAmbientColor" )->AsVector();
			if(pAmbientVariable)
				pAmbientVariable->SetFloatVector((float*)&pLight->m_AmbientColor);

			ID3DX11EffectVectorVariable* pLightSpecularVariable =
				pEffect->GetVariableByName("g_LightSpecular" )->AsVector();
			if(pLightSpecularVariable)
			{
				raVector4 lightSpec = raVector4(1.0f, 1.0f, 1.0f, 1.0f);
				pLightSpecularVariable->SetFloatVector((float*)&lightSpec);
			}
			ID3DX11EffectScalarVariable* pLightSpecularPowerVariable =
				pEffect->GetVariableByName("g_SpecularPower" )->AsScalar();
			if(pLightSpecularPowerVariable)
				pLightSpecularPowerVariable->SetInt(64);

			ID3DX11EffectVectorVariable* pLightPosViewSpaceVariable     =
				pEffect->GetVariableByName("g_LightPosViewSpace" )->AsVector();
			raVector4 lightPosViewSpace;
			D3DXVec4Transform((D3DXVECTOR4*)&lightPosViewSpace, (D3DXVECTOR4*)&lightPos,
				(D3DXMATRIX*)&m_Camera->GetViewMatrix());
			if(pLightPosViewSpaceVariable)
				pLightPosViewSpaceVariable->SetFloatVector((float*)&lightPosViewSpace);

			ID3DX11EffectVectorVariable* pLightDirViewSpaceVariable     =
				pEffect->GetVariableByName("g_LightDirViewSpace" )->AsVector();
			raVector3 lightDirViewSpace;
			D3DXVec3TransformNormal((D3DXVECTOR3*)&lightDirViewSpace, (D3DXVECTOR3*)&lightDir,
				(D3DXMATRIX*)&m_Camera->GetViewMatrix());
			lightDirViewSpace = raVector3Normalize(lightDirViewSpace);
			if(pLightDirViewSpaceVariable)
				pLightDirViewSpaceVariable->SetFloatVector((float*)&lightDirViewSpace);

			ID3DX11EffectVectorVariable* pLightAttenuationVariable     =
				pEffect->GetVariableByName("g_LightAttenuation" )->AsVector();
			if(pLightAttenuationVariable)
				pLightAttenuationVariable->SetFloatVector((float*)&pLight->m_Attenuation);

			//Spotlight
			ID3DX11EffectScalarVariable* pcosHalfPhiVariable     =
				pEffect->GetVariableByName("g_cosHalfPhi" )->AsScalar();
			if(pcosHalfPhiVariable) pcosHalfPhiVariable->SetFloat(
				pLight->m_cosHalfPhi);
			ID3DX11EffectScalarVariable* pcosHalfThetaVariable     =
				pEffect->GetVariableByName("g_cosHalfTheta" )->AsScalar();
			if(pcosHalfThetaVariable) pcosHalfThetaVariable->SetFloat(
				pLight->m_cosHalfTheta);

			for(int i = 0; i < numLights; i++)
			{
				ID3DX11EffectVariable* pLightVariable = pEffect->
					GetVariableByName("g_Lights")->GetElement(i);

				raVector4 lightPos = raVector4(m_Lights[i]->m_Position.x, m_Lights[i]->m_Position.y,
											   m_Lights[i]->m_Position.z , 1.0f);
				pLightVariable->GetMemberByName("Position")->AsVector()->
					SetFloatVector((float*)&lightPos);

				raVector3 lightDir = m_Lights[i]->m_Direction;
				pLightVariable->GetMemberByName("Direction")->AsVector()->
					SetFloatVector((float*)&lightDir);

				raVector4 lightPosViewSpace;
				D3DXVec4Transform((D3DXVECTOR4*)&lightPosViewSpace, (D3DXVECTOR4*)&lightPos,
					(D3DXMATRIX*)&m_Camera->GetViewMatrix());
				pLightVariable->GetMemberByName("PosViewSpace")->AsVector()->
					SetFloatVector((float*)&lightPosViewSpace);

				raVector3 lightDirViewSpace;
				D3DXVec3TransformNormal((D3DXVECTOR3*)&lightDirViewSpace, (D3DXVECTOR3*)&lightDir,
					(D3DXMATRIX*)&m_Camera->GetViewMatrix());
				lightDirViewSpace = raVector3Normalize(lightDirViewSpace);
				pLightVariable->GetMemberByName("DirViewSpace")->AsVector()->
					SetFloatVector((float*)&lightDirViewSpace);

				pLightVariable->GetMemberByName("Attenuation")->AsVector()->
					SetFloatVector((float*)&m_Lights[i]->m_Attenuation);

				pLightVariable->GetMemberByName("cosHalfPhi")->AsScalar()->
					SetFloat(m_Lights[i]->m_cosHalfPhi);
				pLightVariable->GetMemberByName("cosHalfTheta")->AsScalar()->
					SetFloat(m_Lights[i]->m_cosHalfTheta);

				pLightVariable->GetMemberByName("Diffuse")->AsVector()->
					SetFloatVector((float*)&m_Lights[i]->m_DiffuseColor);
				pLightVariable->GetMemberByName("Ambient")->AsVector()->
					SetFloatVector((float*)&m_Lights[i]->m_AmbientColor);
			}
	}
	void raSceneManager::RenderToTexture(ID3D11RenderTargetView* pRTView, ID3D11DepthStencilView* pDSView)
	{
		ID3D11RenderTargetView* pOldRenderTargetViews[1] = { NULL };
		ID3D11DepthStencilView* pOldDepthStencilView = NULL;

		m_dx->GetImmediateContext()->OMGetRenderTargets(1, pOldRenderTargetViews,
			&pOldDepthStencilView);

		if(!pDSView) pDSView = pOldDepthStencilView;

		//Render Target setzen
		ID3D11RenderTargetView* pRenderTargetViews[1] = { pRTView };
		m_dx->GetImmediateContext()->OMSetRenderTargets(1, pRenderTargetViews,
			pDSView);

		float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //Muss schwarz sein, da beim Glow-EFfekt das in die Textur gerenderte Bild additiv über die Szene gelegt wird
		m_dx->GetImmediateContext()->ClearRenderTargetView(pRTView, ClearColor );
		m_dx->GetImmediateContext()->ClearDepthStencilView(pDSView, D3D11_CLEAR_DEPTH, 1.0, 0);

		Render();

		m_dx->GetImmediateContext()->OMSetRenderTargets(1, pOldRenderTargetViews,
			pOldDepthStencilView);

		SAFE_RELEASE( pOldRenderTargetViews[0]);
		SAFE_RELEASE( pOldDepthStencilView);
	}
	void raSceneManager::RenderToTexture(raRenderScreen& renderScreen)
	{
		RenderToTexture(renderScreen.m_pRTView,
			renderScreen.m_pDSView,
			renderScreen.m_Width,
			renderScreen.m_Height);
	}
	void raSceneManager::RenderToTexture(ID3D11RenderTargetView* pRTView, ID3D11DepthStencilView* pDSView,
							UINT width, UINT height)
	{
		D3D11_VIEWPORT OldVP;
		SetViewport(&OldVP, width, height);

		RenderToTexture(pRTView, pDSView);

		m_dx->GetImmediateContext()->RSSetViewports(1, &OldVP);
	}
	void raSceneManager::CreateCubeMapMatrices()
	{
		float fHeight = 0.0f;//1.5f;
		raVector3 vEyePt = raVector3( 0.0f, fHeight, 0.0f );
		raVector3 vLookDir;
		raVector3 vUpDir;
		//0 = rechts, 1 = links, 2 = oben
		//3 = unten, 4 = vorn, 5 = hinten

		vLookDir = raVector3( 1.0f, fHeight, 0.0f );
		vUpDir   = raVector3( 0.0f, 1.0f, 0.0f );
		D3DXMatrixLookAtLH((D3DXMATRIX*)&m_CubeMapViewAdjust[0], (D3DXVECTOR3*)&vEyePt,
			(D3DXVECTOR3*)&vLookDir, (D3DXVECTOR3*)&vUpDir );
		vLookDir = raVector3(-1.0f, fHeight, 0.0f );
		vUpDir   = raVector3( 0.0f, 1.0f, 0.0f );
		D3DXMatrixLookAtLH( (D3DXMATRIX*)&m_CubeMapViewAdjust[1], (D3DXVECTOR3*)&vEyePt,
			(D3DXVECTOR3*)&vLookDir, (D3DXVECTOR3*)&vUpDir );
		vLookDir = raVector3( 0.0f, fHeight+1.0f, 0.0f );
		vUpDir   = raVector3( 0.0f, 0.0f,-1.0f );
		D3DXMatrixLookAtLH( (D3DXMATRIX*)&m_CubeMapViewAdjust[2], (D3DXVECTOR3*)&vEyePt,
			(D3DXVECTOR3*)&vLookDir, (D3DXVECTOR3*)&vUpDir );
		vLookDir = raVector3( 0.0f, fHeight-1.0f, 0.0f );
		vUpDir   = raVector3( 0.0f, 0.0f, 1.0f );
		D3DXMatrixLookAtLH( (D3DXMATRIX*)&m_CubeMapViewAdjust[3], (D3DXVECTOR3*)&vEyePt,
			(D3DXVECTOR3*)&vLookDir, (D3DXVECTOR3*)&vUpDir );
		vLookDir = raVector3( 0.0f, fHeight, 1.0f );
		vUpDir   = raVector3( 0.0f, 1.0f, 0.0f );
		D3DXMatrixLookAtLH( (D3DXMATRIX*)&m_CubeMapViewAdjust[4], (D3DXVECTOR3*)&vEyePt,
			(D3DXVECTOR3*)&vLookDir, (D3DXVECTOR3*)&vUpDir );
		vLookDir = raVector3( 0.0f, fHeight,-1.0f );
		vUpDir   = raVector3( 0.0f, 1.0f, 0.0f );

		D3DXMatrixLookAtLH( (D3DXMATRIX*)&m_CubeMapViewAdjust[5], (D3DXVECTOR3*)&vEyePt,
			(D3DXVECTOR3*)&vLookDir, (D3DXVECTOR3*)&vUpDir );

		D3DXMatrixPerspectiveFovLH( (D3DXMATRIX*)&m_ProjCM, PI * 0.5f, 1.0f, .5f, 1000.f );
	}

	void raSceneManager::SetViewport(D3D11_VIEWPORT* pOldVP, UINT width, UINT height)
	{
		UINT numViewports = 1;
		m_dx->GetImmediateContext()->RSGetViewports(&numViewports, pOldVP);

		D3D11_VIEWPORT  viewport;
		viewport.Height = (float)height;
		viewport.Width = (float)width;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		m_dx->GetImmediateContext()->RSSetViewports(1, &viewport);
	}
	void raSceneManager::RenderToCubeMap(raRenderScreen& renderScreen, raMatrix& worldCenter)
	{
		RenderToCubeMap(renderScreen.m_pRTView,
						renderScreen.m_pDSView,
						renderScreen.m_Width,
						renderScreen.m_Height,
						worldCenter);
	}
	void raSceneManager::RenderToCubeMap(ID3D11RenderTargetView* pRTView, ID3D11DepthStencilView* pDSView,
								UINT width, UINT height, raMatrix& worldCenter)
	{
		D3D11_VIEWPORT OldVP;
		SetViewport(&OldVP, width, height);

		ID3D11RenderTargetView* pOldRenderTargetViews[1] = { NULL };
		ID3D11DepthStencilView* pOldDepthStencilView = NULL;
		m_dx->GetImmediateContext()->OMGetRenderTargets(1, pOldRenderTargetViews,
			&pOldDepthStencilView);

		//Render Target setzen
		ID3D11RenderTargetView* pRenderTargetViews[1] = { pRTView };
		m_dx->GetImmediateContext()->OMSetRenderTargets(1, pRenderTargetViews,
			pDSView);

		float ClearColor[4] = { 0.5, 0.0, 1.0, 0.0 };
		m_dx->GetImmediateContext()->ClearRenderTargetView(pRTView, ClearColor );
		m_dx->GetImmediateContext()->ClearDepthStencilView(pDSView, D3D11_CLEAR_DEPTH, 1.0,
			0);

		//6 View-Matrizen
		raMatrix mViewAlignCM;
		mViewAlignCM = raMatrixIdentity();
		mViewAlignCM.m41 = -worldCenter.m41;
		mViewAlignCM.m42 = -worldCenter.m42;
		mViewAlignCM.m43 = -worldCenter.m43;

		raMatrix ViewCM[6];
		for(int view = 0; view < 6; ++view )
		{
			ViewCM[view] = mViewAlignCM * m_CubeMapViewAdjust[view];
		}

		raEffectMap::iterator iterator;
		for(iterator = raMaterial::m_effects.begin();
			iterator != raMaterial::m_effects.end();
			iterator++)
		{
			ID3DX11Effect* pEffect = iterator->second;
			ID3DX11EffectMatrixVariable* pmViewCM = pEffect->
				GetVariableByName("g_mViewCM" )->AsMatrix();
			if(pmViewCM)
				pmViewCM->SetMatrixArray((float*)ViewCM, 0, 6);
		}

		Render(ViewCM[0], m_ProjCM, &m_Light, "RenderToCubeMap");

		 m_dx->GetImmediateContext()->RSSetViewports( 1, &OldVP );
		 m_dx->GetImmediateContext()->OMSetRenderTargets(1, pOldRenderTargetViews,
			pOldDepthStencilView);

		SAFE_RELEASE( pOldRenderTargetViews[0]);
		SAFE_RELEASE( pOldDepthStencilView);
	}
};