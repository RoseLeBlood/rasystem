#pragma once

	struct RAPI TIMEDFLOATKEYS
	{
		DWORD time;
		DWORD ntfkeyValues;
		raMatrix matrix;
	};
	struct RAPI RAANIMATIONKEYHEADER
	{
		DWORD keyType;
		DWORD nKeys;
	};
	struct RAPI RAANIMATION_FRAME_DATA
	{
		LPCSTR						FrameName;
		RAANIMATIONKEYHEADER*		pHeader;
		raMatrix*					pMatrix;
		std::vector<class raNode*>	SubscrobedObject;
	};

	class RAPI raAnimation
	{
	public:
		raAnimation(raSmartPointer<raDirectX> dx);
		~raAnimation(void);

		void AddFrameData(RAANIMATION_FRAME_DATA* pFrameData);

		virtual bool Update(float fTime, float fElapsedTime);

		void Subscribe(raNode* pObject);

		//Renderable-Überschreibungen
		virtual bool Create(){ return true; };
		virtual void Destroy(){};

		virtual void Render(UINT drawOrder, const raMatrix* pView,
			const raMatrix* pProj, const raLight* pLight,
			LPCSTR techniqueName = ""){};

		virtual void SetVisible (bool bVisible){};
		virtual void SetSelected(bool bSelected){};
		virtual bool Intersects(const raVector3 *pRayPos, const raVector3 *pRayDir,
								float *pDist){ return false;};

		void UpdateFrameData(UINT iTick, UINT subScriptionNo);

		int									m_NumSubscriptions;
		std::vector<raNode*>				SubscribedObjects;

		SDKANIMATION_FILE_HEADER*           m_pAnimationHeader;

	private:
		raFrameMap m_FrameData;
	};