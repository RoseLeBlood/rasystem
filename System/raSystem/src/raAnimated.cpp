#include "..\include\raMain.h"

namespace System
{
	raAnimated::raAnimated(void)
	{
		m_pAnimation = NULL;
		m_startKey = 0;
		m_numKeys = 0;
	}

	UINT raAnimated::GetAnimationKeyFromTime(float fTime)
	{
		UINT aniFPS = m_pAnimation->m_pAnimationHeader->AnimationFPS;
		UINT iTick = (UINT)(aniFPS * fTime);

		iTick = iTick % (m_numKeys - 1);
		iTick += m_startKey;
		return iTick;
	}
	void raAnimated::SetAnimation(UINT clip)
	{
		if(!m_pAnimation->m_pAnimationHeader) return;

		switch(clip)
		{
		case 0: //laufen
			m_startKey = 442;
			m_numKeys = 14;
			break;
		case 1: // Drehen
			m_startKey = 1;
			m_numKeys = 70;
			break;
		case 2: //BackFlip
			m_startKey = 120;
			m_numKeys = 80;
			break;
		case 3: //Kämpfen
			m_startKey = 1164;
			m_numKeys = 28;
			break;
		case 4: //Sterben
			m_startKey = 622;
			m_numKeys = 10;
		break;
		default:
			m_startKey = 442;
			m_numKeys = 14;
			break;
		}
	}
	//--------------------------
	void raAnimatedNode::Update(raFrameMap &m_FrameData, float fRunTime, UINT subScriptionNo)
	{
		UINT iTick = GetAnimationKeyFromTime(fRunTime);
		m_pAnimation->UpdateFrameData(iTick, subScriptionNo);

		raMatrix Identy = raMatrixIdentity();
		TransformFrame(&Identy);
	}
};