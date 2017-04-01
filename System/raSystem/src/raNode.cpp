#include "..\include\raMain.h"
namespace System
{
	raNode::raNode(raSmartPointer<raDirectX> dx, LPCSTR strName, raNode* pParent) : raRender(dx, NULL, strName)
	{
		m_pParent = pParent;
		m_pChild = NULL;
		m_pSibling = NULL;

		m_pMesh  = NULL;
	}

	void raNode::Destroy()
	{
		if(m_pMesh)    m_pMesh->Destroy();
		if(m_pSibling) m_pSibling->Destroy();
		if(m_pChild)   m_pChild->Destroy();
	}

	raNode::~raNode()
	{
		delete m_pMesh;
		delete m_pSibling;
		delete m_pChild;
	}

	void raNode::SetVisual(raVisual* pVisual)
	{
		if(!m_pMesh)m_pMesh = pVisual;
		FindRoot()->m_Meshes.push_back(pVisual);
	}

	void raNode::SetChild(raNode* pChild)
	{
		if(! m_pChild) //erstes Kind
			m_pChild = pChild;
		else
			m_pChild->SetSibling(pChild);
	}

	void raNode::SetLocatorMarkers(raGroup* pGroup)
	{
		if (strstr(m_Name, "Locator"))
		{
			//SetVisual(pVisual);
			pGroup->AddInstance(*GetWorldMatrix());
		}

		//Geschwister
		if(m_pSibling)
			m_pSibling->SetLocatorMarkers(pGroup);

		//Kinder
		if(m_pChild)
			m_pChild->SetLocatorMarkers(pGroup);
	}
	void raNode::SetSelected(bool bSelected)
	{
		if(m_pMesh)
			m_pMesh->SetSelected(bSelected);

		if(m_pSibling)
			m_pSibling->SetSelected(bSelected);

		//Kinder
		if(m_pChild)
			m_pChild->SetSelected(bSelected);
	}
	void raNode::SetSibling(raNode* pSibling)
	{
		if(!m_pSibling) //Erstes Geschwister
			m_pSibling = pSibling;
		else            //Probieren wir's bei den Geschwistern
			m_pSibling->SetSibling(pSibling);
	}

	void raNode::TransformFrame(raMatrix* pParentWorld)
	{
		m_WorldTransformed = m_World * (*pParentWorld);

		if(m_pMesh)
		{
			m_pMesh->SetWorldMatrix(m_WorldTransformed);
		}

		//Geschwister
		if(m_pSibling)
			m_pSibling->TransformFrame(pParentWorld);

		//Kinder
		if(m_pChild)
			m_pChild->TransformFrame(&m_WorldTransformed);
	}

	void raNode::SetWorldMatrix (const raMatrix& value)
	{
		raRender::SetWorldMatrix(value);
		if(m_pParent)
			TransformFrame(m_pParent->GetWorldMatrix()); //Weil nicht in Update aufgerufen wird
		else
		{
			raMatrix identity;
			identity = raMatrixIdentity();
			TransformFrame(&identity);
		}
	}

	void raNode::SetVisible (bool bVisible)
	{
		if(m_pMesh)
			m_pMesh->SetVisible(bVisible);

		//Geschwister
		if(m_pSibling)
			m_pSibling->SetVisible(bVisible);

		//Kinder
		if(m_pChild)
			m_pChild->SetVisible(bVisible);
	}

	bool raNode::Create()
	{
		if(m_pMesh) m_pMesh->Create();

		//Geschwister
		if(m_pSibling)
			m_pSibling->Create();

		//Kinder
		if(m_pChild)
			m_pChild->Create();

		return true;
	}

	bool raNode::Render(UINT drawOrder, const raMatrix& pView, const raMatrix& pProj,
							const raLight* pLight, LPCSTR techniqueName)
	{
		if(m_pMesh)
		{
			m_pMesh->Render(drawOrder, pView, pProj, pLight, techniqueName);
		}

		//Geschwister
		if(m_pSibling)
			m_pSibling->Render(drawOrder, pView, pProj, pLight, techniqueName);

		//Kinder
		if(m_pChild)
			m_pChild->Render(drawOrder, pView, pProj, pLight, techniqueName);

		return true;
	}

	bool raNode::Update(float fTime, float fElapsedTime)
	{
		if(m_pMesh) m_pMesh->Update(fTime, fElapsedTime);

		//Geschwister
		if(m_pSibling)
			m_pSibling->Update(fTime, fElapsedTime);

		//Kinder
		if(m_pChild)
			m_pChild->Update(fTime, fElapsedTime);

		return true;
	}

	raNode*  raNode::FindRoot()
	{
		if(m_pParent) return m_pParent->FindRoot();
		return this;
	}

	raNode*  raNode::FindNode(LPCSTR strName)
	{
		if(!strcmp(m_Name, strName))
		{
			return this;
		}

		//Geschwister
		if(m_pSibling)
		{
			raNode* praNode = m_pSibling->FindNode(strName);
			if(praNode) return praNode;
		}

		//Kinder
		if(m_pChild)
		{
			raNode* praNode = m_pChild->FindNode(strName);
			if(praNode) return praNode;
		}

		return NULL;
	}

	bool raNode::Intersects(const raVector3* pRayPos,
								const raVector3* pRayDir,
								float* pDist)
	{
		bool bResult = false;
		float dist = FLT_MAX;

		if(m_pMesh)
		{
			raVector3 rayPos;
			raVector3 rayDir;

			raMatrix inverseWorld;
			D3DXMatrixInverse((D3DXMATRIX *)&inverseWorld, NULL, (D3DXMATRIX *)&m_WorldTransformed);
			D3DXVec3TransformCoord ((D3DXVECTOR3*)&rayPos, (D3DXVECTOR3*)pRayPos, (D3DXMATRIX*)&inverseWorld);
			D3DXVec3TransformNormal((D3DXVECTOR3*)&rayDir, (D3DXVECTOR3*)pRayDir, (D3DXMATRIX*)&inverseWorld);

			if(m_pMesh->Intersects(&rayPos, &rayDir, &dist))
			{
				bResult = true;
				if(dist < *pDist)
					*pDist = dist;
			}
		}

		if(m_pSibling)
		{
			if(m_pSibling->Intersects(pRayPos, pRayDir, &dist))
			{
				bResult = true;
				if(dist < *pDist)
					*pDist = dist;
			}
		}

		if(m_pChild)
		{
			if(m_pChild->Intersects(pRayPos, pRayDir, &dist))
			{
				bResult = true;
				if(dist < *pDist)
					*pDist = dist;
			}
		}
		return bResult;
	}

	void raNode::TransformFrame(raMatrix* pParentWorld, UINT instanceNo)
	{
		m_WorldTransformed = m_World *  (*pParentWorld);

	   if(m_pMesh)
		{
			((raGroup* )m_pMesh)->SetWorldMatrix(m_WorldTransformed, instanceNo);
		}

		//Geschwister
		if(m_pSibling)
			m_pSibling->TransformFrame(pParentWorld, instanceNo);

		//Kinder
		if(m_pChild)
			m_pChild->TransformFrame(&m_WorldTransformed, instanceNo);
	}
};