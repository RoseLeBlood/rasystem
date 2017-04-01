#include "..\include\raMain.h"

namespace System
{
	raAnimation::raAnimation(raSmartPointer<raDirectX> dx) //: raRender(dx, 0, "raAnimation")
	{
		m_pAnimationHeader = new SDKANIMATION_FILE_HEADER();
		m_NumSubscriptions = 0;
	}
	raAnimation::~raAnimation(void)
	{
		SAFE_DELETE(m_pAnimationHeader);
		raFrameMap::iterator it;
		for(it = m_FrameData.begin(); it != m_FrameData.end(); it++)
		{
			System::RAANIMATION_FRAME_DATA* pFrameData = it->second;
			SAFE_DELETE_ARRAY(pFrameData->pMatrix);
			SAFE_DELETE(pFrameData);
		}
	}

	void raAnimation::AddFrameData(RAANIMATION_FRAME_DATA* pFrameData)
	{
		m_FrameData.insert(raFrameMap::value_type(pFrameData->FrameName,
			pFrameData));
	}

	bool raAnimation::Update(float fTime, float fElapsedTime)
	{
		for(UINT i = 0; i < SubscribedObjects.size(); i++)
		{
			raNode*  pRoot = SubscribedObjects[i];
			pRoot->Update(m_FrameData, fTime, i);
		}
		return true;
	}

	void raAnimation::UpdateFrameData(UINT iTick, UINT subScriptionNo)
	{
		raFrameMap::iterator it;
		for(it = m_FrameData.begin(); it != m_FrameData.end(); it++)
		{
			RAANIMATION_FRAME_DATA* pFrameData = it->second;
			pFrameData->SubscrobedObject[subScriptionNo]->
				raRender::SetWorldMatrix(pFrameData->pMatrix[iTick]);
		}
	}

	void raAnimation::Subscribe(raNode* pObject)
	{
		raFrameMap::iterator it = m_FrameData.find(pObject->GetName());
		if(it != m_FrameData.end())
		{
			it->second->SubscrobedObject.push_back(pObject);
		}
	}
	raAnimatedNode::raAnimatedNode(raSmartPointer<raDirectX> dx, LPCSTR strName, raNode* pParent)
		: raNode(dx, strName, pParent) {} //Standardkonstruktor von Animated wird automat. aufgerufen
};