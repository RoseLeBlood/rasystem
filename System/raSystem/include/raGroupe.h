#pragma once

#define MAX_INSTANCES 500

	class RAPI raGroup : public raVisual
	{
	public:
		raGroup(raSmartPointer<raDirectX> dx, IRAMesh *pVisual, int numInstancesToDraw = 0,
			bool bAlpha = false, bool bAdditive = false, bool bDepthDisable = false,
			D3D11_FILL_MODE fillMode = D3D11_FILL_SOLID,
			D3D11_CULL_MODE cullMode = D3D11_CULL_BACK);

		virtual ~raGroup(void);

		virtual bool Create();
		virtual void Destroy();
		virtual bool CreateMesh() { return m_pVisual->CreateMesh();}
		virtual bool DestroyMesh(){ return m_pVisual->DestroyMesh();}
		virtual bool RenderMesh(LPCSTR techniqueName = "");
		virtual void SetWorldMatrix(const raMatrix& value);
		virtual void SetWorldMatrix(const raMatrix& value, int instanceNo);
		virtual const raMatrix* GetWorldMatrix(int instanceNo);
		virtual bool Intersects(CONST raVector3 *pRayPos,
			CONST raVector3 *pRayDir, FLOAT *pDist)						{ return false; }
		virtual ID3D11Buffer* GetVertexBuffer()							{ return m_pVisual->GetVertexBuffer(); }
		virtual ID3D11Buffer* GetIndexBuffer()							{ return m_pVisual->GetIndexBuffer(); }
		virtual UINT GetStrideSize()									{ return m_pVisual->GetStrideSize(); }
		virtual D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(UINT n)	{ return m_pVisual->GetPrimitiveTopology(n); }
		virtual UINT GetNumSubsets()									{ return m_pVisual->GetNumSubsets();}
		virtual SDKMESH_SUBSET* GetSubset(UINT n)						{ return m_pVisual->GetSubset(n);}

		void CreateRandomMatrices(float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f, float offsetY = 0.0f);
		UINT AddInstance(const raMatrix& world);

		IRAMesh* GetVisual(void)							{ return m_pVisual; }

	protected:
		virtual void		LoadInstanceData();
		virtual bool		CreateVertexLayout();
		void				InitInstanceMatrices();
		void				UpdateInstanceMatrices();
	protected:
		IRAMesh				*m_pVisual;
		raMatrix			m_InstanceMatrices[MAX_INSTANCES];
		int					m_iNumInstances;
		int					m_iNumInstancesToDraw;
		raMatrix*			m_pInstanceList;

		ID3D11Buffer*		m_pInstanceData;
	};