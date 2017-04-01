#include "..\include\raMain.h"
namespace System
{
	 raOctree::raOctree(raVector3& bbMin, raVector3& bbMax,  raOctree* pParent):
			m_bbMin(bbMin), m_bbMax(bbMax)
		{
			//Rückwärts durchlaufen, damit Punkte hinten zuerst entfernt werden
			for (int n = pParent->m_Points.GetSize() - 3; n >= 0; n -= 3)
			{
				if(
					IsPointInOctant(pParent->m_Points[n    ]) &&
					IsPointInOctant(pParent->m_Points[n + 1]) &&
					IsPointInOctant(pParent->m_Points[n + 2]) )
				{
					m_Points.Add(pParent->m_Points[n]);
					m_Points.Add(pParent->m_Points[n + 1]);
					m_Points.Add(pParent->m_Points[n + 2]);
					pParent->m_Points.Remove(n + 2);
					pParent->m_Points.Remove(n + 1);
					pParent->m_Points.Remove(n);
				}
			}

			if((((float)m_bbMax.x - (float)m_bbMin.x) > 1) && (m_Points.GetSize() > 0))
			{
				CreateChildNodes();
			}
			else
				m_pChildren = NULL;
	   }

		void raOctree::CreateChildNodes()
		{
			m_pChildren = new raOctree*[8];
			int i = 0;
			for(int x = -1; x <= 1; x+=2)
			{
				for(int y = -1; y <= 1; y+=2)
				{
					for(int z = -1; z <= 1; z+=2)
					{
						raVector3 bbMin, bbMax;

						bbMin.x = 0.25f * x * (((float)m_bbMax.x - (float)m_bbMin.x)
							+ ((float)m_bbMax.x + 3 * (float)m_bbMin.x));
						bbMin.y = 0.25f * y * (((float)m_bbMax.y - (float)m_bbMin.y)
							+ ((float)m_bbMax.y + 3 * (float)m_bbMin.y));
						bbMin.z = 0.25f * z * (((float)m_bbMax.z - (float)m_bbMin.z)
							+ ((float)m_bbMax.z + 3 * (float)m_bbMin.z));

						bbMax.x = 0.25f * x * (((float)m_bbMax.x - (float)m_bbMin.x)
							+ (3 * (float)m_bbMax.x + (float)m_bbMin.x));
						bbMax.y = 0.25f * y * (((float)m_bbMax.y - (float)m_bbMin.y)
							+ (3 * (float)m_bbMax.y + (float)m_bbMin.y));
						bbMax.z = 0.25f * z * (((float)m_bbMax.z - (float)m_bbMin.z)
							+ (3 * (float)m_bbMax.z + (float)m_bbMin.z));

						m_pChildren[i++] = new raOctree(bbMin, bbMax, this);
					}
				}
			}
		}

		bool raOctree::IsPointInOctant(raVector3 p)
		{
			return ( (p.x >= m_bbMin.x && p.x <= m_bbMax.x) &&
					 (p.y >= m_bbMin.y && p.y <= m_bbMax.y) &&
					 (p.z >= m_bbMin.z && p.z <= m_bbMax.z));
		}

		raOctree::~raOctree(void)
		{
			if(m_pChildren)
			{
				for(int i = 0; i < 8; i++)
				{
					delete m_pChildren[i];
				}
				delete[] m_pChildren;
			}
		}

		bool raOctree::Intersects(const raVector3* pRayPos,
								const raVector3* pRayDir,
								float* pDist)
		{
			if(! D3DXBoxBoundProbe((D3DXVECTOR3*)&m_bbMin, (D3DXVECTOR3*)&m_bbMax, (D3DXVECTOR3*)pRayPos, (D3DXVECTOR3*)pRayDir))
				return false;

			bool bIntersects = false;
			float dist = FLT_MAX;
			float mindist = FLT_MAX;

			//Zuerst die eigenen Dreiecke untersuchen
			float u = 0, v = 0; //u, v nicht verwendet
			for (int n = 0; n < m_Points.GetSize(); n += 3)
			{
				if(D3DXIntersectTri((D3DXVECTOR3 *)&m_Points[n], (D3DXVECTOR3 *)&m_Points[n + 1], (D3DXVECTOR3 *)&m_Points[n + 2],
					(D3DXVECTOR3 *)pRayPos, (D3DXVECTOR3 *)pRayDir, &u, &v, &dist))
				{
					bIntersects = true;
					if(dist < mindist)
					{
						mindist = dist;
					}
				}
			}

			//Jetzt ggf. die Kinder
			if(m_pChildren)
			{
				for(int i = 0; i < 8; i++)
				{
					if (m_pChildren[i]->Intersects(pRayPos, pRayDir, &dist))
					{
						bIntersects = true;
						if(dist < mindist)
						{
							mindist = dist;
						}
				   }
				}
			}
			*pDist = mindist;
			return bIntersects;
		}
};