#pragma once

	class RAPI raAnimated
	{
	public:
		raAnimated();
		UINT GetAnimationKeyFromTime(float fTime);
		void SetAnimation(UINT clip);

		UINT m_startKey;
		UINT m_numKeys;
		class raAnimation* m_pAnimation;
	};

	class RAPI raAnimatedNode: public raNode, public raAnimated
	{
	public:
		raAnimatedNode(raSmartPointer<raDirectX> dx, LPCSTR strName = "raAnimatedNode", raNode* pParent = NULL);

		virtual void Update(raFrameMap &m_FrameData, float fTime, UINT subScriptionNo);
	};