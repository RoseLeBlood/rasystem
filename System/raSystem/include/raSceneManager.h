#pragma once

	class raRenderScreen;
	class raTexturedMaterial;
	class raSound;

	class RAPI raSceneManager
	{
	public:
		raSceneManager(raSmartPointer<raCamera> Camera);
		~raSceneManager(void);

		bool Add(raRender* Object);
		bool Remove(raRender* Object);
		bool RemoveAt(int i);

		void SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj);
		void SetupLights(ID3DX11Effect* pEffect, const raLight* pLight);
		void Render(const raMatrix& pView, const raMatrix &pProj, const raLight* pLight, LPCSTR techniqueName = "");

		virtual bool Create(raSmartPointer<raDirectX> dx);
		virtual void Destroy();
		virtual void Render();
		virtual void Update(float fTime, float fRunTime);

		int			GetNumItems()								{ return (UINT)m_Visuals.size(); }
		raRender*	GetItem(int index);
		raVector3	GetLightPosition()							{ return m_Light.m_Position; }

		void	SetLightDirection(const raVector3& Dir)			{ m_Light.m_Direction = Dir; }
		void	SetLightAmbientColor(const raColor& Amb)		{ m_Light.m_AmbientColor = Amb; }
		void	SetLightDiffuseColor(const raColor& Dif)		{ m_Light.m_DiffuseColor = Dif; }
		void	SetLightPosition(const raVector3& Pos)			{ m_Light.m_Position = Pos; }
		void    ShowAllObjects(bool show);

		void	RenderToTexture(raRenderScreen& renderScreen);
		void	RenderToTextureWithGlobalMaterial(raRenderScreen& renderScreen,LPCSTR techniqueName = "");
		void	RenderToCubeMap(raRenderScreen& renderScreen, raMatrix& worldCenter);
		void	RenderToLightMap();
		void	RenderToLightMap(raRenderScreen& renderScreen);

		void TranslateObject(LPCSTR objectName, float x, float y, float z){};
		void RotateObjectY(LPCSTR objectName, float angle){};

		void SetLightProjParams(float fFOV, float fAspect, float fNearPlane, float fFarPlane)
		{
			m_Light.SetProjParams(fFOV, fAspect, fNearPlane, fFarPlane);
		}
		void SetCamera(raSmartPointer<raCamera> pCamera)
		{
			m_Camera = pCamera;
		}
		void SetLightPosition(int no, float x, float y, float z)
		{
			m_Lights[no]->m_Position = raVector3(x, y, z);
		};

		void SetLightDirection(int no, float x, float y, float z)
		{
			m_Lights[no]->m_Direction = raVector3(x, y, z);
		};

		void SetLightAmbientColor(int no, float x, float y, float z)
		{
			m_Lights[no]->m_AmbientColor = COLOR_XRGB(x, y, z);
		};

		void SetLightDiffuseColor(int no, float x, float y, float z)
		{
			m_Lights[no]->m_DiffuseColor = COLOR_XRGB(x, y, z);
		};

	private:
		void	CreateCubeMapMatrices();
		void	SetViewport(D3D11_VIEWPORT* pOldVP, UINT width, UINT height);
		void	RenderToTexture(ID3D11RenderTargetView* pRTView, ID3D11DepthStencilView* pDSView,
								UINT width, UINT height);
		void	RenderToTexture(ID3D11RenderTargetView* pRTView, ID3D11DepthStencilView* pDSView = NULL);
		void	RenderToCubeMap(ID3D11RenderTargetView* pRTView, ID3D11DepthStencilView* pDSView,
								UINT width, UINT height, raMatrix& worldCenter);

		void	RenderToTextureWithGlobalMaterial( ID3D11RenderTargetView* pRTView, ID3D11DepthStencilView* pDSView,
												   UINT width, UINT height,
												   LPCSTR techniqueName = "");
		void	RenderToLightMap(ID3D11RenderTargetView* pRTView,
								 ID3D11DepthStencilView* pDSView,
								 UINT width, UINT height);

	private:
		raSmartPointer<raDirectX>				m_dx;
		bool					m_bCreated;

		std::vector<raRender*>	m_Visuals;
		raSmartPointer<raCamera>				m_Camera;
		static const int		numLights = 5;
		raLight*				m_Lights[numLights]; //Hier können mehrere Lichtquellen eingefügt werden
		raLight					m_Light;			 //Standardlichtquelle

		raMatrix				m_CubeMapViewAdjust[6];
		raMatrix				m_ProjCM;
		raTexturedMaterial*		m_pShadowReceiverMaterial;
	};