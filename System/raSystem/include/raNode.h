#pragma once

	class raGroup;
	// {0B57EDFD-5EDE-4C92-BCD5-F1A7A52710EB}
	DEFINE_GUID(RAIID_raNode,
	0xb57edfd, 0x5ede, 0x4c92, 0xbc, 0xd5, 0xf1, 0xa7, 0xa5, 0x27, 0x10, 0xeb);

	class RAPI raNode : public raRender
	{
	protected:
		raNode(const GUID& raguid, raSmartPointer<raDirectX> dx, LPCSTR strName = "raNode", raNode* pParent = NULL);
	public:
		raNode(raSmartPointer<raDirectX> dx, LPCSTR strName = "raNode", raNode* pParent = NULL);
		~raNode();

		void SetChild(raNode* pChild);
		void SetSibling(raNode* pSibling);

		virtual raMatrix*  GetWorldMatrix()	{ return &m_WorldTransformed; }
		virtual void SetWorldMatrix	(const raMatrix& value);

		virtual bool Create();
		virtual void Destroy();
		virtual bool Render(UINT drawOrder, const raMatrix& pView, const raMatrix& pProj,
							const raLight* pLight, LPCSTR techniqueName = "");
		virtual bool Update(float fTime, float fElapsedTime);

		virtual void SetVisible (bool bVisible);
		virtual void SetSelected(bool bSelected);

		virtual void Update(raFrameMap &m_FrameData, float fTime, UINT subScriptionNo){};

		virtual void TransformFrame(raMatrix* pParentWorld);
		virtual void TransformFrame(raMatrix* pParentWorld, UINT instanceNo);

		raNode*  FindNode(LPCSTR strName);
		raNode*  FindRoot();

		raVisual* GetVisual(){return m_pMesh;}
		void SetVisual(raVisual* pVisual);
		raVisual* GetVisual(UINT n ){ return FindRoot()->m_Meshes[n];}
		UINT GetNumVisuals(){ return (UINT)FindRoot()->m_Meshes.size();}

		virtual bool Intersects(const raVector3* pRayPos,
								const raVector3* pRayDir,
								float* pDist);

		void SetLocatorMarkers(raGroup* pGroup);

	protected:
		raMatrix  m_WorldTransformed;

		raNode* m_pParent;
		raNode* m_pSibling;
		raNode* m_pChild;

		raVisual* m_pMesh;

	public:
		std::vector<raVisual*> m_Meshes; //Wurzelknoten jeder Hierarchie führt Liste der Meshes
									   //Vorsicht beim Einfügen von Kindern, die vorher selbst Wurzelknoten waren!
	};