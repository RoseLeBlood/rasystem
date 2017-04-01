#pragma once

	class RAPI raOrthoCamera : public raCamera
	{
	public:
		raOrthoCamera(void);
		~raOrthoCamera(void);

		virtual void SetProjParams(FLOAT w, FLOAT h, FLOAT fNearPlane, FLOAT fFarPlane);
		virtual void SetViewParams(raVector3* pvEyePt, raVector3* pvLookatPt);

		virtual void FrameMove(float fElapsedTime, float FPS ){}
	};