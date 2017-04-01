//--------------------------------------------------------------------------------------
// File: DXUTcamera.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------
#include "..\include\raMain.h"
#undef min // use __min instead
#undef max // use __max instead

namespace System
{
//--------------------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------------------
raCamera::raCamera()
{
    m_cKeysDown = 0;
    ZeroMemory( m_aKeys, sizeof( BYTE ) * CAM_MAX_KEYS );

    // Set attributes for the view matrix
    raVector3 vEyePt = raVector3( 0.0f, 0.0f, 0.0f );
    raVector3 vLookatPt = raVector3( 0.0f, 0.0f, 1.0f );

    // Setup the view matrix
    SetViewParams( &vEyePt, &vLookatPt );

    // Setup the projection matrix
    SetProjParams( PI / 4.0f, 1.0f, 1.0f, 1000.0f );

    GetCursorPos( &m_ptLastMousePosition );
    m_bMouseLButtonDown = false;
    m_bMouseMButtonDown = false;
    m_bMouseRButtonDown = false;
    m_nCurrentButtonMask = 0;
    m_nMouseWheelDelta = 0;

    m_fCameraYawAngle = 0.0f;
    m_fCameraPitchAngle = 0.0f;

    SetRect( &m_rcDrag, LONG_MIN, LONG_MIN, LONG_MAX, LONG_MAX );
    m_vVelocity = raVector3( 0, 0, 0 );
    m_bMovementDrag = false;
    m_vVelocityDrag = raVector3( 0, 0, 0 );
    m_fDragTimer = 0.0f;
    m_fTotalDragTimeToZero = 0.25;
    m_vRotVelocity = raVector2( 0, 0 );

    m_fRotationScaler = 0.01f;
    m_fMoveScaler = 5.0f;

    m_bInvertPitch = false;
    m_bEnableYAxisMovement = true;
    m_bEnablePositionMovement = true;

    m_vMouseDelta = raVector2( 0, 0 );
    m_fFramesToSmoothMouseData = 2.0f;

    m_bClipToBoundary = false;
    m_vMinBoundary = raVector3( -1, -1, -1 );
    m_vMaxBoundary = raVector3( 1, 1, 1 );

    m_bResetCursorAfterMove = false;
}

//--------------------------------------------------------------------------------------
// Client can call this to change the position and direction of camera
//--------------------------------------------------------------------------------------
VOID raCamera::SetViewParams( raVector3* pvEyePt, raVector3* pvLookatPt )
{
    if( NULL == pvEyePt || NULL == pvLookatPt )
        return;

    m_vDefaultEye = m_vEye = *pvEyePt;
    m_vDefaultLookAt = m_vLookAt = *pvLookatPt;

    // Calc the view matrix
    raVector3 vUp( 0,1,0 );
    D3DXMatrixLookAtLH((D3DXMATRIX*)&m_mView, (D3DXVECTOR3*)pvEyePt, (D3DXVECTOR3*)pvLookatPt, (D3DXVECTOR3*)&vUp );

    D3DXMATRIX mInvView;
    D3DXMatrixInverse( &mInvView, NULL, (D3DXMATRIX *)&m_mView );

    // The axis basis vectors and camera position are stored inside the
    // position matrix in the 4 rows of the camera's world matrix.
    // To figure out the yaw/pitch of the camera, we just need the Z basis vector
   raVector3* pZBasis = ( raVector3* )&mInvView._31;

    m_fCameraYawAngle = atan2f( pZBasis->x, pZBasis->z );
    float fLen = sqrtf( pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x );
    m_fCameraPitchAngle = -atan2f( pZBasis->y, fLen );
}

//--------------------------------------------------------------------------------------
// Calculates the projection matrix based on input params
//--------------------------------------------------------------------------------------
VOID raCamera::SetProjParams( FLOAT fFOV, FLOAT fAspect, FLOAT fNearPlane,
                                 FLOAT fFarPlane )
{
    // Set attributes for the projection matrix
    m_fFOV = fFOV;
    m_fAspect = fAspect;
    m_fNearPlane = fNearPlane;
    m_fFarPlane = fFarPlane;
    D3DXMatrixPerspectiveFovLH( (D3DXMATRIX*)&m_mProj, fFOV, fAspect, fNearPlane, fFarPlane );
}
void raCamera::SetOrthoCamera( raVector2* pvWindowSize, float fNearPlane, float fFarPlane)
{
	D3DXMatrixOrthoLH((D3DXMATRIX*)&m_mOrtho, pvWindowSize->x, pvWindowSize->y, fNearPlane, fFarPlane);
}
long raCamera::OnKeyDown(HWND wnd, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER( lParam );
	m_hwnd = wnd;

	D3DUtil_CameraKeys mappedKey = MapKey( ( UINT )wParam );
    if( mappedKey != CAM_UNKNOWN )
    {
        if( FALSE == IsKeyDown( m_aKeys[mappedKey] ) )
        {
            m_aKeys[ mappedKey ] = KEY_WAS_DOWN_MASK | KEY_IS_DOWN_MASK;
            ++m_cKeysDown;
        }
    }
	return 0;
}
long raCamera::OnKeyUp(HWND wnd, WPARAM wParam, LPARAM lParam)
{
	// Map this key to a D3DUtil_CameraKeys enum and update the
    // state of m_aKeys[] by removing the KEY_IS_DOWN_MASK mask.
    D3DUtil_CameraKeys mappedKey = MapKey( ( UINT )wParam  );
    if( mappedKey != CAM_UNKNOWN && ( DWORD )mappedKey < 8 )
    {
        m_aKeys[ mappedKey ] &= ~KEY_IS_DOWN_MASK;
        --m_cKeysDown;
    }
	return 0;
}
long raCamera::OnRMouseUp(HWND wnd, WPARAM wParam, LPARAM lParam)
{
	m_bMouseRButtonDown = false; m_nCurrentButtonMask &= ~MOUSE_RIGHT_BUTTON;
	if( !m_bMouseLButtonDown &&
        !m_bMouseRButtonDown &&
        !m_bMouseMButtonDown )
		{
			ReleaseCapture();
		}

	return 0;
}
long raCamera::OnRMouseDown(HWND wnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptCursor =
    {
        ( short )LOWORD( lParam ), ( short )HIWORD(lParam )
    };
	m_bMouseRButtonDown = true; m_nCurrentButtonMask |= MOUSE_RIGHT_BUTTON;
	SetCapture( wnd);
    GetCursorPos( &m_ptLastMousePosition );

	return 0;
}
long raCamera::OnLMouseUp(HWND wnd, WPARAM wParam, LPARAM lParam)
{
	m_bMouseLButtonDown = false; m_nCurrentButtonMask &= ~MOUSE_LEFT_BUTTON;
	if( !m_bMouseLButtonDown &&
        !m_bMouseRButtonDown &&
        !m_bMouseMButtonDown )
		{
			ReleaseCapture();
		}

	return 0;
}
long raCamera::OnLMouseDown(HWND wnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptCursor =
    {
        ( short )LOWORD( lParam ), ( short )HIWORD(lParam )
    };
	m_bMouseLButtonDown = true; m_nCurrentButtonMask |= MOUSE_LEFT_BUTTON;
	SetCapture( wnd);
    GetCursorPos( &m_ptLastMousePosition );
	return 0;
}
long raCamera::OnMMouseUp(HWND wnd, WPARAM wParam, LPARAM lParam)
{
	m_bMouseMButtonDown = false; m_nCurrentButtonMask &= ~MOUSE_MIDDLE_BUTTON;
	if( !m_bMouseLButtonDown &&
        !m_bMouseRButtonDown &&
        !m_bMouseMButtonDown )
		{
			ReleaseCapture();
		}
	return 0;
}
long raCamera::OnMMouseDown(HWND wnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptCursor =
    {
        ( short )LOWORD( lParam ), ( short )HIWORD(lParam )
    };
	 m_bMouseMButtonDown = true; m_nCurrentButtonMask |= MOUSE_MIDDLE_BUTTON;
	 SetCapture( wnd );
     GetCursorPos( &m_ptLastMousePosition );

	 return 0;
}
long raCamera::OnCaptureChange(HWND wnd, WPARAM wParam, LPARAM lParam)
{
	 if( ( HWND )lParam != wnd )
    {
        if( ( m_nCurrentButtonMask & MOUSE_LEFT_BUTTON ) ||
            ( m_nCurrentButtonMask & MOUSE_MIDDLE_BUTTON ) ||
            ( m_nCurrentButtonMask & MOUSE_RIGHT_BUTTON ) )
        {
            m_bMouseLButtonDown = false;
            m_bMouseMButtonDown = false;
            m_bMouseRButtonDown = false;
            m_nCurrentButtonMask &= ~MOUSE_LEFT_BUTTON;
            m_nCurrentButtonMask &= ~MOUSE_MIDDLE_BUTTON;
            m_nCurrentButtonMask &= ~MOUSE_RIGHT_BUTTON;
            ReleaseCapture();
        }
    }
	 return 0;
}
long raCamera::OnMouseWheel(HWND wnd, WPARAM wParam, LPARAM lParam)
{
	m_nMouseWheelDelta += ( short )HIWORD( wParam );
	return 0;
}

//--------------------------------------------------------------------------------------
// Figure out the velocity based on keyboard input & drag if any
//--------------------------------------------------------------------------------------
void raCamera::GetInput( bool bGetKeyboardInput, bool bGetMouseInput, bool bGetGamepadInput,
                            bool bResetCursorAfterMove )
{
    m_vKeyboardDirection = raVector3( 0, 0, 0 );
    if( bGetKeyboardInput )
    {
        // Update acceleration vector based on keyboard state
        if( IsKeyDown( m_aKeys[CAM_MOVE_FORWARD] ) )
            m_vKeyboardDirection.z += 1.0f;
        if( IsKeyDown( m_aKeys[CAM_MOVE_BACKWARD] ) )
            m_vKeyboardDirection.z -= 1.0f;
        if( m_bEnableYAxisMovement )
        {
            if( IsKeyDown( m_aKeys[CAM_MOVE_UP] ) )
                m_vKeyboardDirection.y += 1.0f;
            if( IsKeyDown( m_aKeys[CAM_MOVE_DOWN] ) )
                m_vKeyboardDirection.y -= 1.0f;
        }
        if( IsKeyDown( m_aKeys[CAM_STRAFE_RIGHT] ) )
            m_vKeyboardDirection.x += 1.0f;
        if( IsKeyDown( m_aKeys[CAM_STRAFE_LEFT] ) )
            m_vKeyboardDirection.x -= 1.0f;
    }

    if( bGetMouseInput )
    {
        UpdateMouseDelta();
    }
}

//--------------------------------------------------------------------------------------
// Figure out the mouse delta based on mouse movement
//--------------------------------------------------------------------------------------
void raCamera::UpdateMouseDelta()
{
    POINT ptCurMouseDelta;
    POINT ptCurMousePos;

    // Get current position of mouse
    GetCursorPos( &ptCurMousePos );

    // Calc how far it's moved since last frame
    ptCurMouseDelta.x = ptCurMousePos.x - m_ptLastMousePosition.x;
    ptCurMouseDelta.y = ptCurMousePos.y - m_ptLastMousePosition.y;

    // Record current position for next time
    m_ptLastMousePosition = ptCurMousePos;

    if( m_bResetCursorAfterMove)
    {
        // Set position of camera to center of desktop,
        // so it always has room to move.  This is very useful
        // if the cursor is hidden.  If this isn't done and cursor is hidden,
        // then invisible cursor will hit the edge of the screen
        // and the user can't tell what happened
        POINT ptCenter;

        // Get the center of the current monitor
        MONITORINFO mi;
        mi.cbSize = sizeof( MONITORINFO );
        GetMonitorInfo( MonitorFromWindow( m_hwnd, MONITOR_DEFAULTTONEAREST ), &mi );
        ptCenter.x = ( mi.rcMonitor.left + mi.rcMonitor.right ) / 2;
        ptCenter.y = ( mi.rcMonitor.top + mi.rcMonitor.bottom ) / 2;
        SetCursorPos( ptCenter.x, ptCenter.y );
        m_ptLastMousePosition = ptCenter;
    }

    // Smooth the relative mouse data over a few frames so it isn't
    // jerky when moving slowly at low frame rates.
    float fPercentOfNew = 1.0f / m_fFramesToSmoothMouseData;
    float fPercentOfOld = 1.0f - fPercentOfNew;
    m_vMouseDelta.x = m_vMouseDelta.x * (raFloat)fPercentOfOld + (raFloat)ptCurMouseDelta.x * (raFloat)fPercentOfNew;
    m_vMouseDelta.y = m_vMouseDelta.y * (raFloat)fPercentOfOld + (raFloat)ptCurMouseDelta.y * (raFloat)fPercentOfNew;

    m_vRotVelocity = m_vMouseDelta * m_fRotationScaler;
}

//--------------------------------------------------------------------------------------
// Figure out the velocity based on keyboard input & drag if any
//--------------------------------------------------------------------------------------
void raCamera::UpdateVelocity( float fElapsedTime )
{
    raVector2 vGamePadRightThumb = raVector2( m_vGamePadRightThumb.x, - (raFloat)m_vGamePadRightThumb.z );
    m_vRotVelocity = m_vMouseDelta * m_fRotationScaler + vGamePadRightThumb * 0.02f * fElapsedTime;

    raVector3 vAccel = m_vKeyboardDirection + m_vGamePadLeftThumb;

    // Normalize vector so if moving 2 dirs (left & forward),
    // the camera doesn't move faster than if moving in 1 dir
    vAccel = raVector3NormalizeEx(vAccel );

    // Scale the acceleration vector
    vAccel *= m_fMoveScaler;

    if( m_bMovementDrag )
    {
        // Is there any acceleration this frame?
        if( raVector3LenghtSq(vAccel ) > 0 )
        {
            // If so, then this means the user has pressed a movement key\
            // so change the velocity immediately to acceleration
            // upon keyboard input.  This isn't normal physics
            // but it will give a quick response to keyboard input
            m_vVelocity = vAccel;
            m_fDragTimer = m_fTotalDragTimeToZero;
            m_vVelocityDrag = vAccel / m_fDragTimer;
        }
        else
        {
            // If no key being pressed, then slowly decrease velocity to 0
            if( m_fDragTimer > 0 )
            {
                // Drag until timer is <= 0
                m_vVelocity -= m_vVelocityDrag * fElapsedTime;
                m_fDragTimer -= fElapsedTime;
            }
            else
            {
                // Zero velocity
                m_vVelocity = raVector3( 0, 0, 0 );
            }
        }
    }
    else
    {
        // No drag, so immediately change the velocity
        m_vVelocity = vAccel;
    }
}

//--------------------------------------------------------------------------------------
// Clamps pV to lie inside m_vMinBoundary & m_vMaxBoundary
//--------------------------------------------------------------------------------------
void raCamera::ConstrainToBoundary( raVector3* pV )
{
    // Constrain vector to a bounding box
    pV->x = __max( pV->x, m_vMinBoundary.x );
    pV->y = __max( pV->y, m_vMinBoundary.y );
    pV->z = __max( pV->z, m_vMinBoundary.z );

    pV->x = __min( pV->x, m_vMaxBoundary.x );
    pV->y = __min( pV->y, m_vMaxBoundary.y );
    pV->z = __min( pV->z, m_vMaxBoundary.z );
}

//--------------------------------------------------------------------------------------
// Maps a windows virtual key to an enum
//--------------------------------------------------------------------------------------
D3DUtil_CameraKeys raCamera::MapKey( UINT nKey )
{
    // This could be upgraded to a method that's user-definable but for
    // simplicity, we'll use a hardcoded mapping.
    switch( nKey )
    {
        case VK_CONTROL:
            return CAM_CONTROLDOWN;
        case VK_LEFT:
            return CAM_STRAFE_LEFT;
        case VK_RIGHT:
            return CAM_STRAFE_RIGHT;
        case VK_UP:
            return CAM_MOVE_FORWARD;
        case VK_DOWN:
            return CAM_MOVE_BACKWARD;
        case VK_PRIOR:
            return CAM_MOVE_UP;        // pgup
        case VK_NEXT:
            return CAM_MOVE_DOWN;      // pgdn

        case 'A':
            return CAM_STRAFE_LEFT;
        case 'D':
            return CAM_STRAFE_RIGHT;
        case 'W':
            return CAM_MOVE_FORWARD;
        case 'S':
            return CAM_MOVE_BACKWARD;
        case 'Q':
            return CAM_MOVE_DOWN;
        case 'E':
            return CAM_MOVE_UP;

        case VK_NUMPAD4:
            return CAM_STRAFE_LEFT;
        case VK_NUMPAD6:
            return CAM_STRAFE_RIGHT;
        case VK_NUMPAD8:
            return CAM_MOVE_FORWARD;
        case VK_NUMPAD2:
            return CAM_MOVE_BACKWARD;
        case VK_NUMPAD9:
            return CAM_MOVE_UP;
        case VK_NUMPAD3:
            return CAM_MOVE_DOWN;

        case VK_F12:
            return CAM_RESET;
    }

    return CAM_UNKNOWN;
}

//--------------------------------------------------------------------------------------
// Reset the camera's position back to the default
//--------------------------------------------------------------------------------------
VOID raCamera::Reset()
{
    SetViewParams(&m_vDefaultEye, &m_vDefaultLookAt );
}

//--------------------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------------------
raCameraFP::raCameraFP() : m_nActiveButtonMask( 0x07 ), raCamera()
{
    m_bRotateWithoutButtonDown = false;
}

//--------------------------------------------------------------------------------------
// Update the view matrix based on user input & elapsed time
//--------------------------------------------------------------------------------------
VOID raCameraFP::FrameMove(float fElapsedTime, float FPS )
{
    if( IsKeyDown( m_aKeys[CAM_RESET] ) )
        Reset();

    // Get keyboard/mouse/gamepad input
    GetInput( m_bEnablePositionMovement, ( m_nActiveButtonMask & m_nCurrentButtonMask ) || m_bRotateWithoutButtonDown,
              true, m_bResetCursorAfterMove );

    UpdateVelocity( fElapsedTime );

    // Simple euler method to calculate position delta
    raVector3 vPosDelta = m_vVelocity * fElapsedTime;

    // If rotating the camera
    if( ( m_nActiveButtonMask & m_nCurrentButtonMask ) ||
        m_bRotateWithoutButtonDown ||
        (float)m_vGamePadRightThumb.x != 0.0f ||
        (float)m_vGamePadRightThumb.z != 0.0f )
    {
        // Update the pitch & yaw angle based on mouse movement
        float fYawDelta = m_vRotVelocity.x;
        float fPitchDelta = m_vRotVelocity.y;

        // Invert pitch if requested
        if( m_bInvertPitch )
            fPitchDelta = -fPitchDelta;

        m_fCameraPitchAngle += fPitchDelta;
        m_fCameraYawAngle += fYawDelta;

        // Limit pitch to straight up or straight down
        m_fCameraPitchAngle = __max( -PI / 2.0f, m_fCameraPitchAngle );
        m_fCameraPitchAngle = __min( +PI / 2.0f, m_fCameraPitchAngle );
    }

    // Make a rotation matrix based on the camera's yaw & pitch
    D3DXMATRIX mCameraRot;
    D3DXMatrixRotationYawPitchRoll( &mCameraRot, m_fCameraYawAngle, m_fCameraPitchAngle, 0 );

    // Transform vectors based on camera's rotation matrix
    D3DXVECTOR3 vWorldUp, vWorldAhead;
    D3DXVECTOR3 vLocalUp = D3DXVECTOR3( 0, 1, 0 );
    D3DXVECTOR3 vLocalAhead = D3DXVECTOR3( 0, 0, 1 );
    D3DXVec3TransformCoord( &vWorldUp, &vLocalUp, &mCameraRot );
    D3DXVec3TransformCoord( &vWorldAhead, &vLocalAhead, &mCameraRot );

    // Transform the position delta by the camera's rotation
    D3DXVECTOR3 vPosDeltaWorld;
    if( !m_bEnableYAxisMovement )
    {
        // If restricting Y movement, do not include pitch
        // when transforming position delta vector.
        D3DXMatrixRotationYawPitchRoll( &mCameraRot, m_fCameraYawAngle, 0.0f, 0.0f );
    }
    D3DXVec3TransformCoord( &vPosDeltaWorld, (D3DXVECTOR3 *)&vPosDelta, (D3DXMATRIX *)&mCameraRot );

    // Move the eye position
    m_vEye += vPosDeltaWorld;

    if( m_bClipToBoundary )
        ConstrainToBoundary(&m_vEye );

    // Update the lookAt position based on the eye position
    m_vLookAt = m_vEye + vWorldAhead;

    // Update the view matrix
    D3DXMatrixLookAtLH( (D3DXMATRIX *)&m_mView, (D3DXVECTOR3*)&m_vEye, (D3DXVECTOR3*)&m_vLookAt, (D3DXVECTOR3*)&vWorldUp );

    D3DXMatrixInverse((D3DXMATRIX *) &m_mCameraWorld, NULL,(D3DXMATRIX *) &m_mView );
}

//--------------------------------------------------------------------------------------
// Enable or disable each of the mouse buttons for rotation drag.
//--------------------------------------------------------------------------------------
void raCameraFP::SetRotateButtons( bool bLeft, bool bMiddle, bool bRight, bool bRotateWithoutButtonDown )
{
    m_nActiveButtonMask = ( bLeft ? MOUSE_LEFT_BUTTON : 0 ) |
        ( bMiddle ? MOUSE_MIDDLE_BUTTON : 0 ) |
        ( bRight ? MOUSE_RIGHT_BUTTON : 0 );
    m_bRotateWithoutButtonDown = bRotateWithoutButtonDown;
}
//-----------------------------------------------------------------------
void raColCamera::FrameMove( float fElapsedTime, float FPS )
{
	if( IsKeyDown( m_aKeys[CAM_RESET] ) )
        Reset();

    // Get keyboard/mouse/gamepad input
    GetInput( m_bEnablePositionMovement, ( m_nActiveButtonMask & m_nCurrentButtonMask ) || m_bRotateWithoutButtonDown,
              true, m_bResetCursorAfterMove );

    UpdateVelocity( fElapsedTime );

    // Simple euler method to calculate position delta
    raVector3 vPosDelta = m_vVelocity * fElapsedTime;

    // If rotating the camera
    if( ( m_nActiveButtonMask & m_nCurrentButtonMask ) ||
        m_bRotateWithoutButtonDown ||
        (float)m_vGamePadRightThumb.x != 0.0f ||
        (float)m_vGamePadRightThumb.z != 0.0f )
    {
        // Update the pitch & yaw angle based on mouse movement
        float fYawDelta = m_vRotVelocity.x;
        float fPitchDelta = m_vRotVelocity.y;

        // Invert pitch if requested
        if( m_bInvertPitch )
            fPitchDelta = -fPitchDelta;

        m_fCameraPitchAngle += fPitchDelta;
        m_fCameraYawAngle += fYawDelta;

        // Limit pitch to straight up or straight down
        m_fCameraPitchAngle = __max( -PI / 2.0f, m_fCameraPitchAngle );
        m_fCameraPitchAngle = __min( +PI / 2.0f, m_fCameraPitchAngle );
    }

    // Make a rotation matrix based on the camera's yaw & pitch
    D3DXMATRIX mCameraRot;
    D3DXMatrixRotationYawPitchRoll( &mCameraRot, m_fCameraYawAngle, m_fCameraPitchAngle, 0 );

	 // Transform vectors based on camera's rotation matrix
    D3DXVECTOR3 vWorldUp, vWorldAhead;
    D3DXVECTOR3 vLocalUp = D3DXVECTOR3( 0, 1, 0 );
    D3DXVECTOR3 vLocalAhead = D3DXVECTOR3( 0, 0, 1 );
    D3DXVec3TransformCoord( &vWorldUp, &vLocalUp, &mCameraRot );
    D3DXVec3TransformCoord( &vWorldAhead, &vLocalAhead, &mCameraRot );

	  // Transform the position delta by the camera's rotation
    D3DXVECTOR3 vPosDeltaWorld;
    if( !m_bEnableYAxisMovement )
    {
        // If restricting Y movement, do not include pitch
        // when transforming position delta vector.
        D3DXMatrixRotationYawPitchRoll( &mCameraRot, m_fCameraYawAngle, 0.0f, 0.0f );
    }
    D3DXVec3TransformCoord( &vPosDeltaWorld, (D3DXVECTOR3 *)&vPosDelta, (D3DXMATRIX *)&mCameraRot );

	m_cDist = FLT_MAX;
	//raColObject temp;
	bool Move = true;

	for(int i = 0; i < m_pObject.GetSize(); i++)
	{
		raColObject temp = m_pObject.GetAt(i);
		if(temp.Object)
		{
			raVector3 rayDir;
			rayDir = raVector3Normalize(vPosDeltaWorld);
			temp.Object->Intersects(&m_vEye, &rayDir, &m_cDist);
		}
		int itemp = (int)(raVector3Lenght(vPosDeltaWorld) + temp.Distance + m_fNearPlane);
		if((m_cDist >= itemp) && (Move == true))
		{
			// Move the eye position
			m_vEye += vPosDeltaWorld;

			if( m_bClipToBoundary )
				ConstrainToBoundary(&m_vEye );
		}
		else
			Move = false;
	}

	// Update the lookAt position based on the eye position
	m_vLookAt = m_vEye + vWorldAhead;
	D3DXMatrixLookAtLH( (D3DXMATRIX *)&m_mView, (D3DXVECTOR3*)&m_vEye, (D3DXVECTOR3*)&m_vLookAt, (D3DXVECTOR3*)&vWorldUp );

    D3DXMatrixInverse((D3DXMATRIX *) &m_mCameraWorld, NULL,(D3DXMATRIX *) &m_mView );
}
};