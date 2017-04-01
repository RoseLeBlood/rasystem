#pragma warning( disable : 4127 )

	template<class VertexType, bool bIndexed = true, typename IndexType = UINT16>
		class raEntity : public raVisual
		{
			friend class raOctree;
		public:
			raEntity(raSmartPointer<raDirectX>,
				raMaterial*  pMaterial,  bool bAlpha = false, bool bAdditive = false,
				UINT numMaterials = 1);
			/*virtual */~raEntity();

			virtual bool CreateMesh();
			virtual bool DestroyMesh();

			virtual bool RenderMesh(LPCSTR techniqueName = "");

			virtual bool Intersects(const raVector3* pRayPos,
									const raVector3* pRayDir,
									float* pDist);

			virtual ID3D11Buffer* GetVertexBuffer(){ return m_pVB;}
			virtual ID3D11Buffer* GetIndexBuffer() { return m_pIB;}
			virtual UINT GetStrideSize(){return sizeof(VertexType);}
			virtual D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(UINT n)
			{
				return (D3D11_PRIMITIVE_TOPOLOGY) m_pSubsets[n].PrimitiveType;
			}
			virtual SDKMESH_SUBSET* GetSubset(UINT n){ return &m_pSubsets[n];}
			virtual UINT GetNumSubsets(){ return m_numSubsets;}

		protected:
			ID3D11Buffer* m_pVB;
			ID3D11Buffer* m_pIB;

			DWORD m_nVertices;
			DWORD m_nIndices;

			VertexType* m_pVertices;
			IndexType* m_pIndices;

			raOctree*	m_OctreeRoot;

			virtual void SetupVertices() = 0;
			virtual void SetupIndices(){ };

			virtual void CreateVertexBuffer();
			virtual void CreateIndexBuffer();
			virtual void DeleteTempData();

			virtual const D3D11_INPUT_ELEMENT_DESC* GetVertexLayout()
			{
				return VertexType::GetVertexLayout();
			}

			virtual UINT GetNumElements()
			{
				return VertexType::GetNumElements();
			}

			virtual DXGI_FORMAT GetIndexBufferFormat()
			{
				if(sizeof(IndexType) == 2)
					return DXGI_FORMAT_R16_UINT;
				else
					return DXGI_FORMAT_R32_UINT;
			}

			virtual void ComputeBoundingBox();
		};

		template<class VertexType, bool bIndexed, typename IndexType>
		raEntity<VertexType, bIndexed, IndexType>::raEntity(
			raSmartPointer<raDirectX> dx,
			raMaterial*  pMaterial, bool bAlpha, bool bAdditive,
			UINT numMaterials):
		raVisual(dx, bAlpha, bAdditive)
		{
			m_numMaterials = numMaterials;
			if (numMaterials > 0)
			{
				m_pMaterials = new raMaterial*[numMaterials];
				m_pMaterials[0] = pMaterial;

				m_numSubsets = 1;
				m_pSubsets = new SDKMESH_SUBSET[m_numSubsets];
				for(UINT n = 0; n < m_numSubsets; n++)
				{
					m_pSubsets[n].IndexStart = 0;
					m_pSubsets[n].IndexCount = 0;
					m_pSubsets[n].VertexStart = 0;
					m_pSubsets[n].VertexCount = 0;
					m_pSubsets[n].MaterialID = 0;
					m_pSubsets[n].PrimitiveType =
						D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				}
			}
			m_nVertices = 0;
			m_nIndices = 0;

			m_pVertices = NULL;
			m_pIndices = NULL;

			m_pVB = NULL;
			m_pIB = NULL;
			m_OctreeRoot = NULL;
		}

		template<class VertexType, bool bIndexed, typename IndexType>
		bool raEntity<VertexType, bIndexed, IndexType>::DestroyMesh()
		{
			SAFE_RELEASE(m_pVB);
			SAFE_RELEASE(m_pIB);

			return true;
		}
		template<class VertexType, bool bIndexed, typename IndexType>
		void raEntity<VertexType, bIndexed, IndexType>::CreateVertexBuffer()
		{
			SetupVertices();

			if(m_pSubsets)
			{
				m_pSubsets[0].VertexStart = 0;
				m_pSubsets[0].VertexCount = m_nVertices;
			}

			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = m_nVertices * GetStrideSize();
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = m_pVertices;
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;

			m_dx->GetDevice()->CreateBuffer(&bufferDesc, &InitData, &m_pVB);
		}

		template<class VertexType, bool bIndexed, typename IndexType>
		void raEntity<VertexType, bIndexed, IndexType>::CreateIndexBuffer()
		{
			if(!bIndexed) return;
			SetupIndices();

			//Index Buffer
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = m_nIndices * sizeof(IndexType);
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = m_pIndices;
			m_dx->GetDevice()->CreateBuffer( &bufferDesc, &InitData, &m_pIB);
		}

		template<class VertexType, bool bIndexed, typename IndexType>
		raEntity<VertexType, bIndexed, IndexType>::~raEntity()
		{
			DeleteTempData();
			SAFE_DELETE(m_OctreeRoot);
		}

		template<class VertexType, bool bIndexed, typename IndexType>
		bool raEntity<VertexType, bIndexed, IndexType>::CreateMesh()
		{
			CreateVertexBuffer();
			CreateIndexBuffer();

			ComputeBoundingBox();
			m_OctreeRoot = new raOctree(this);
			return true;
		 }

		template<class VertexType, bool bIndexed, typename IndexType>
		void raEntity<VertexType, bIndexed, IndexType>::DeleteTempData()
		{
			SAFE_DELETE_ARRAY(m_pVertices);
			if(bIndexed) SAFE_DELETE_ARRAY(m_pIndices);
		}

		template<class VertexType, bool bIndexed, typename IndexType>
		bool raEntity<VertexType, bIndexed, IndexType>::RenderMesh(LPCSTR techniqueName)
		{
			UINT Strides[1];
			UINT Offsets[1];
			Strides[0] = GetStrideSize();
			Offsets[0] = 0;
			m_dx->GetImmediateContext()->IASetVertexBuffers(0, 1, &m_pVB, Strides, Offsets );

			if(bIndexed)
				m_dx->GetImmediateContext()->IASetIndexBuffer(m_pIB, GetIndexBufferFormat(), 0);

			for (DWORD n = 0; n < m_numMaterials; n++)
			{
				UINT indexStart;
				UINT indexCount;

				if(m_pSubsets)
				{
					indexStart = (UINT)m_pSubsets[n].IndexStart;
					indexCount = (UINT)m_pSubsets[n].IndexCount;
					m_pMaterials[m_pSubsets[n].MaterialID]->Setup();
					m_dx->GetImmediateContext()->IASetPrimitiveTopology(GetPrimitiveTopology(n));
				}
				else
				{
					assert(false);
					indexStart = 0;
					indexCount = m_nIndices;
					m_pMaterials[0]->Setup();
					m_dx->GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				}

				ID3DX11EffectTechnique* pTechnique =
					m_pMaterials[n]->GetEffectTechnique(techniqueName);

				if(pTechnique)
				{
					if(pTechnique->IsValid())
					{
						D3DX11_TECHNIQUE_DESC techDesc;
						pTechnique->GetDesc(&techDesc);

						for(UINT p = 0; p < techDesc.Passes; ++p )
						{
							pTechnique->GetPassByIndex(p)->Apply(0, m_dx->GetImmediateContext());
							if(bIndexed)
								m_dx->GetImmediateContext()->DrawIndexed(indexCount, indexStart, 0);
							else
								m_dx->GetImmediateContext()->Draw(m_nVertices, 0);
						}
					}
				}
			}
			return true;
		}
		template<class VertexType, bool bIndexed, typename IndexType>
		void raEntity<VertexType, bIndexed, IndexType>::ComputeBoundingBox()
		{
			for(UINT i = 0; i < m_nVertices; i++)
			{
				if(m_pVertices[i].position.x < m_bbMin.x)
					m_bbMin.x = m_pVertices[i].position.x;
				if(m_pVertices[i].position.y < m_bbMin.y)
					m_bbMin.y = m_pVertices[i].position.y;
				if(m_pVertices[i].position.z < m_bbMin.z)
					m_bbMin.z = m_pVertices[i].position.z;

				if(m_pVertices[i].position.x > m_bbMax.x)
					m_bbMax.x = m_pVertices[i].position.x;
				if(m_pVertices[i].position.y > m_bbMax.y)
					m_bbMax.y = m_pVertices[i].position.y;
				if(m_pVertices[i].position.z > m_bbMax.z)
					m_bbMax.z = m_pVertices[i].position.z;
		   }
		}
		template<class VertexType, bool bIndexed, typename IndexType>
		bool raEntity<VertexType, bIndexed, IndexType>::Intersects(const raVector3* pRayPos,
									const raVector3* pRayDir,
									float* pDist)
		{
			if(*pRayDir == raVector3(0, 0, 0))return false;

			if(!m_OctreeRoot)
			{
				bool bIntersects = false;
				float mindist = FLT_MAX;
				float u = 0, v = 0, dist = FLT_MAX; //u, v nicht verwendet

				for (UINT64 idx = 0; idx < m_nIndices; idx += 3)
				{
					raVector3 p[3];
					for(int i = 0; i < 3; i++)
						p[i] = ((VertexType*)m_pVertices +
						m_pIndices[idx + i])->position;

					if(D3DXIntersectTri((D3DXVECTOR3*)&p[0], (D3DXVECTOR3*)&p[1], (D3DXVECTOR3*)&p[2],
						(D3DXVECTOR3*)pRayPos,(D3DXVECTOR3*) pRayDir, &u, &v, &dist))
					{
						bIntersects = true;
						if(dist < mindist)
						{
							mindist = dist;
						}
					}
				}
				*pDist = mindist;
				return bIntersects;
			}
			else
			{
				return m_OctreeRoot->Intersects(pRayPos, pRayDir, pDist);
			}
		}