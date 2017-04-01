#pragma once

	class RAPI raWater : public raVisual
	{
	public:
		raWater(raSmartPointer<raDirectX> dx, int width, int depth, raMaterial *mat);
		~raWater(void);

		virtual bool RenderMesh(LPCSTR techniqueName = "");
		virtual bool Update(float fTime, float fRunTime);

		void push(float x, float y, float depth);
		void pushX(float depth);
		void pushZ(float deth);
		void _PREP(float depth, float x, float y, int addx, int addy);

		virtual bool Intersects(const raVector3* pRayPos,
								const raVector3* pRayDir,
								float* pDist){return false; }

		virtual bool CreateMesh()
		{
			CreateVertexBuffer();
			CreateIndexBuffer();
			return true;
		};

		virtual bool DestroyMesh()
		{
			for(int i = 0; i < 3; i++)
			{
				SAFE_RELEASE (m_pVB[i]);
			}
			SAFE_RELEASE(m_pIB);
			return true;
		};

		virtual ID3D11Buffer* GetVertexBuffer()								{ return m_pVB[m_curBuffNo];}
		virtual ID3D11Buffer* GetIndexBuffer()								{ return m_pIB;}
		virtual D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(UINT n)		{ return (D3D11_PRIMITIVE_TOPOLOGY) m_pSubsets[n].PrimitiveType; }
		virtual DXGI_FORMAT GetIndexBufferFormat()							{ return DXGI_FORMAT_R16_UINT; }
		virtual UINT GetNumSubsets()										{ return m_numSubsets;}
		virtual SDKMESH_SUBSET* GetSubset(UINT n)							{ return NULL;} //Keine Subsets
		virtual UINT GetNumMaterials()										{ return m_numMaterials; }
		virtual raMaterial* GetMaterial(UINT n)								{ return m_pMaterials[n]; }

	protected:
		virtual void SetupVertices();
		virtual void SetupIndices();
		virtual bool CreateVertexLayout();
		virtual void CreateVertexBuffer();
		virtual void CreateIndexBuffer();

	private:
		void CalculateNormals();
	private:
		LPCSTR m_TextureFile;
		LPCSTR m_CubeTextureFile;

		ID3D11Buffer* m_pVB[3]; // Position, Normale, TexCoord
		ID3D11Buffer* m_pIB;

		UINT		  m_nVertices;
		UINT		  m_nIndices;
		int			  m_width, m_depth;

		raVector3*	  m_pVertices[3];
		raVector3*	  m_pNormals;
		raVector2*	  m_pTexcoords;
		UINT16*		  m_pIndices;
		int			  m_curBuffNo;

		float		  m_lastTimeStamp;
		float		  m_lastAnimationTimeStamp;
		float		  m_lastFrameTime;

		static const int ANIMATIONS_PER_SECOND = 15;
	};