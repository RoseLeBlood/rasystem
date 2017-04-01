#pragma once

namespace raSystem
{
	class RAPI raCEGUI : public raRender
	{
	public:
		raCEGUI(raDirectX* dx);
		virtual ~raCEGUI(void) {};

		virtual bool Create();
		virtual void Destroy();
		virtual bool Render(UINT drawOrder, const raMatrix& pView, const raMatrix& pProj, 
							const raLight* pLight, LPCSTR techniqueName = "");

		virtual bool Update(float fTime, float fRunTime){ return true; }

		virtual void SetVisible (bool bVisible) {}; 
		virtual void SetSelected(bool bSelected) {}; 

		virtual const float GetBoundingsphereRadius()		{ return 1.0f; }
		virtual bool		Intersects(const raVector3* pRayPos,
									   const raVector3* pRayDir,
									   float* pDist) { return false; };

	protected:
		HWND					   m_hwnd;
		raVector2				   m_Windsize;
	};
}
	
