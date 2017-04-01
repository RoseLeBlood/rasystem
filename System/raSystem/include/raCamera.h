//--------------------------------------------------------------------------------------
// File: Camera.h
//
// Helper functions for Direct3D programming.
//
// Copyright (c) Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------
#pragma once

	//--------------------------------------------------------------------------------------
	// used by CCamera to map WM_KEYDOWN keys
	//--------------------------------------------------------------------------------------
	enum D3DUtil_CameraKeys
	{
		CAM_STRAFE_LEFT = 0,
		CAM_STRAFE_RIGHT,
		CAM_MOVE_FORWARD,
		CAM_MOVE_BACKWARD,
		CAM_MOVE_UP,
		CAM_MOVE_DOWN,
		CAM_RESET,
		CAM_CONTROLDOWN,
		CAM_MAX_KEYS,
		CAM_UNKNOWN     = 0xFF
	};

	#define KEY_WAS_DOWN_MASK 0x80
	#define KEY_IS_DOWN_MASK  0x01

	#define MOUSE_LEFT_BUTTON   RMOUSE_LBUTTON
	#define MOUSE_MIDDLE_BUTTON RMOUSE_MBUTTON
	#define MOUSE_RIGHT_BUTTON  RMOUSE_RBUTTON
	#define MOUSE_WHEEL         0x08

	class RAPI raCamera
	{
	public:
		raCamera();

		// Call these from client and use Get*Matrix() to read new matrices
		virtual long				OnKeyDown(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long				OnKeyUp(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long				OnRMouseUp(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long				OnRMouseDown(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long				OnLMouseUp(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long				OnLMouseDown(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long				OnMMouseUp(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long				OnMMouseDown(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long				OnCaptureChange(HWND wnd, WPARAM wParam, LPARAM lParam);
		virtual long				OnMouseWheel(HWND wnd, WPARAM wParam, LPARAM lParam);

		virtual void                FrameMove( float fTime, float FPS ) =0;
		/*{
			// Wichtig für 3D Sound - Sollte am Ende aufgerufen werdem!!
			D3DXMatrixInverse((D3DXMATRIX *) &m_mCameraWorld, NULL,(D3DXMATRIX *) &m_mView );
		}*/

		// Functions to change camera matrices
		virtual void                Reset();
		virtual void                SetViewParams( raVector3* pvEyePt, raVector3* pvLookatPt );
		virtual void                SetProjParams( float fFOV, float fAspect, float fNearPlane, float fFarPlane );
		virtual void				SetOrthoCamera( raVector2* pvWindowSize, float fNearPlane, float fFarPlane);
		// Functions to change behavior
		virtual void                SetDragRect( RECT& rc )
		{
			m_rcDrag = rc;
		}
		void                        SetInvertPitch( bool bInvertPitch )
		{
			m_bInvertPitch = bInvertPitch;
		}
		void                        SetDrag( bool bMovementDrag, FLOAT fTotalDragTimeToZero = 0.25f )
		{
			m_bMovementDrag = bMovementDrag; m_fTotalDragTimeToZero = fTotalDragTimeToZero;
		}
		void                        SetEnableYAxisMovement( bool bEnableYAxisMovement )
		{
			m_bEnableYAxisMovement = bEnableYAxisMovement;
		}
		void                        SetEnablePositionMovement( bool bEnablePositionMovement )
		{
			m_bEnablePositionMovement = bEnablePositionMovement;
		}
		void                        SetClipToBoundary( bool bClipToBoundary, D3DXVECTOR3* pvMinBoundary,
													   raVector3* pvMaxBoundary )
		{
			m_bClipToBoundary = bClipToBoundary; if( pvMinBoundary ) m_vMinBoundary = *pvMinBoundary;
			if( pvMaxBoundary ) m_vMaxBoundary = *pvMaxBoundary;
		}
		void                        SetScalers( FLOAT fRotationScaler = 0.01f, FLOAT fMoveScaler = 5.0f )
		{
			m_fRotationScaler = fRotationScaler; m_fMoveScaler = fMoveScaler;
		}
		void                        SetNumberOfFramesToSmoothMouseData( int nFrames )
		{
			if( nFrames > 0 ) m_fFramesToSmoothMouseData = ( float )nFrames;
		}
		void                        SetResetCursorAfterMove( bool bResetCursorAfterMove )
		{
			m_bResetCursorAfterMove = bResetCursorAfterMove;
		}

		// Functions to get state
		virtual const raMatrix GetViewMatrix()
		{
			return raMatrix(m_mView);
		}
		virtual const raMatrix GetProjMatrix()
		{
			return raMatrix(m_mProj);
		}
		virtual const raMatrix GetOrthoMatrix()
		{
			return raMatrix(m_mOrtho);
		}
		virtual raVector3* GetEyePt()
		{
			return &raVector3(m_vEye);
		}
		virtual raVector3* GetLookAtPt()
		{
			return &raVector3(m_vLookAt);
		}
		virtual float GetNearClip()
		{
			return m_fNearPlane;
		}
		virtual float GetFarClip()
		{
			return m_fFarPlane;
		}

		virtual raMatrix *GetWorldMatrix()
		{
			return &m_mCameraWorld;
		}

		virtual raVector3 *GetWorldRight()
		{
			return (raVector3*)&m_mCameraWorld.m11;
		}
		virtual raVector3 *GetWorldUp()
		{
			return (raVector3*)&m_mCameraWorld.m21;
		}
		virtual raVector3 *GetWorldAhead()
		{
			return (raVector3*)&m_mCameraWorld.m31;
		}
		raVector3 *GetVelocity()
		{
			return &m_vVelocity;
		}

		bool IsBeingDragged()
		{
			return ( m_bMouseLButtonDown || m_bMouseMButtonDown || m_bMouseRButtonDown );
		}
		bool IsMouseLButtonDown()
		{
			return m_bMouseLButtonDown;
		}
		bool IsMouseMButtonDown()
		{
			return m_bMouseMButtonDown;
		}
		bool IsMouseRButtonDown()
		{
			return m_bMouseRButtonDown;
		}

	protected:
		// Functions to map a WM_KEYDOWN key to a D3DUtil_CameraKeys enum
		virtual D3DUtil_CameraKeys  MapKey( UINT nKey );
		bool                        IsKeyDown( BYTE key ) const
		{
			return( ( key & KEY_IS_DOWN_MASK ) == KEY_IS_DOWN_MASK );
		}
		bool                        WasKeyDown( BYTE key ) const
		{
			return( ( key & KEY_WAS_DOWN_MASK ) == KEY_WAS_DOWN_MASK );
		}

		void                        ConstrainToBoundary( raVector3* pV );
		void                        UpdateMouseDelta();
		void                        UpdateVelocity( float fElapsedTime );
		void                        GetInput( bool bGetKeyboardInput, bool bGetMouseInput, bool bGetGamepadInput,
											  bool bResetCursorAfterMove );

	protected:
		raMatrix						m_mView;
		raMatrix						m_mCameraWorld;
		raMatrix						m_mProj;
		raMatrix						m_mOrtho;
		raVector3						m_vGamePadLeftThumb;
		raVector3						m_vGamePadRightThumb;
		double							m_GamePadLastActive[4];

		int								m_cKeysDown;
		BYTE							m_aKeys[CAM_MAX_KEYS];
		raVector3						m_vKeyboardDirection;
		POINT							m_ptLastMousePosition;
		bool							m_bMouseLButtonDown;
		bool							m_bMouseMButtonDown;
		bool							m_bMouseRButtonDown;
		int								m_nCurrentButtonMask;
		int								m_nMouseWheelDelta;
		raVector2						m_vMouseDelta;
		float							m_fFramesToSmoothMouseData;

		raVector3						m_vDefaultEye;
		raVector3						m_vDefaultLookAt;
		raVector3						m_vEye;
		raVector3						m_vLookAt;
		float							m_fCameraYawAngle;
		float							m_fCameraPitchAngle;

		RECT							m_rcDrag;
		raVector3						m_vVelocity;
		bool							m_bMovementDrag;
		raVector3						m_vVelocityDrag;
		FLOAT							m_fDragTimer;
		FLOAT							m_fTotalDragTimeToZero;
		raVector2						m_vRotVelocity;

		float							m_fFOV;
		float							m_fAspect;
		float							m_fNearPlane;
		float							m_fFarPlane;

		float							m_fRotationScaler;
		float							m_fMoveScaler;

		bool							m_bInvertPitch;
		bool							m_bEnablePositionMovement;
		bool							m_bEnableYAxisMovement;

		bool							m_bClipToBoundary;
		raVector3						m_vMinBoundary;
		raVector3						m_vMaxBoundary;

		bool							m_bResetCursorAfterMove;
		HWND							m_hwnd ;
	};

	class RAPI raCameraFP : public raCamera
	{
	public:
		raCameraFP();

		virtual void    FrameMove( float fTime, float FPS );
		void            SetRotateButtons( bool bLeft, bool bMiddle, bool bRight, bool bRotateWithoutButtonDown = false );

		raVector3 *GetEyePt()
		{
			return (raVector3*)&m_mCameraWorld.m41;
		}
	protected:
		int m_nActiveButtonMask;  // Mask to determine which button to enable for rotation
		bool m_bRotateWithoutButtonDown;
	};

	class RAPI raColObject
	{
	public:
		raRender*		Object;
		float			Distance;
	};

	typedef raArray<raColObject> raObjectArray;

	class RAPI raColCamera : public raCameraFP
	{
	public:
		raColCamera() : raCameraFP()
		{
			m_Dist = 0.0f;
		}
		virtual void FrameMove( float fTime, float FPS );

		void AddCollisionObject(raRender *pObject, float CoisionDistance)
		{
			raColObject Object = { pObject, CoisionDistance };
			m_pObject.Add(Object);
		}
		float GetCurrentDistance()
		{
			return m_cDist;
		}
	private:
		raObjectArray			m_pObject;
		float					m_Dist;
		float					m_cDist;
	};