#pragma once

	class RAPI raOctree
	{
	public:
		template<class VertexType, bool bIndexed, typename IndexType>
		raOctree(const raEntity<VertexType, bIndexed, IndexType> *pEntity);

		raOctree(raVector3& bbMin, raVector3& bbMax,  raOctree* pParent);
		~raOctree(void);

		virtual bool Intersects(const raVector3* pRayPos,
								const raVector3* pRayDir,
								float* pDist);

	private:
		raVector3 m_bbMin;
		raVector3 m_bbMax;

		raOctree** m_pChildren;
		raArray<raVector3> m_Points;

		bool IsPointInOctant(raVector3 p);
		void CreateChildNodes();
	};
	template<class VertexType, bool bIndexed, typename IndexType>
	raOctree::raOctree(const raEntity<VertexType, bIndexed, IndexType> *pEntity) :
			m_bbMin(pEntity->m_bbMin), m_bbMax(pEntity->m_bbMax)
	{
		for (UINT64 idx = 0; idx < pEntity->m_nIndices; idx += 3)
		{
			raVector3 p[3];
			for(int i = 0; i < 3; i++)
			{
				p[i] = ((VertexType*)pEntity->m_pVertices +
					pEntity->m_pIndices[idx + i])->position;
				m_Points.Add(p[i]);
			}
		}
		CreateChildNodes();
	}