#include "..\include\raMain.h"

namespace System
{
raOrthoCamera::raOrthoCamera(void)
{
}

raOrthoCamera::~raOrthoCamera(void)
{
}

void raOrthoCamera::SetProjParams(FLOAT w, FLOAT h, FLOAT fNearPlane, FLOAT fFarPlane)
{
    m_fFOV        = 0;
    m_fAspect     = w / h;
    m_fNearPlane  = fNearPlane;
    m_fFarPlane   = fFarPlane;
    D3DXMatrixOrthoLH((D3DXMATRIX*)&m_mProj, w, h, fNearPlane, fFarPlane);
}

VOID raOrthoCamera::SetViewParams(raVector3* pvEyePt, raVector3* pvLookatPt)
{
    if( NULL == pvEyePt || NULL == pvLookatPt )
        return;

    m_vDefaultEye = m_vEye = *pvEyePt;
    m_vDefaultLookAt = m_vLookAt = *pvLookatPt;

    raVector3 vUp( 0,1,0 );

	//Falls gewählte Blickrichtung parallel zu vUp ist, vUp neu wählen
	raVector3 viewDir;
	D3DXVec3Normalize((D3DXVECTOR3*)&viewDir,
		D3DXVec3Subtract((D3DXVECTOR3*)&viewDir, (D3DXVECTOR3*)pvLookatPt, (D3DXVECTOR3*)pvEyePt));
	if(1 - abs(D3DXVec3Dot((D3DXVECTOR3*)&vUp, (D3DXVECTOR3*)&viewDir)) <= 0.0001f)
		vUp = D3DXVECTOR3(1, 0, 0);

    D3DXMatrixLookAtLH((D3DXMATRIX *) &m_mView,(D3DXVECTOR3*) pvEyePt,(D3DXVECTOR3*) pvLookatPt, (D3DXVECTOR3*)&vUp );

    D3DXMATRIX mInvView;
    D3DXMatrixInverse((D3DXMATRIX *)&mInvView, NULL,(D3DXMATRIX *) &m_mView );

    D3DXVECTOR3* pZBasis = ( D3DXVECTOR3* )&mInvView._31;

    m_fCameraYawAngle = atan2f( pZBasis->x, pZBasis->z );
    float fLen = sqrtf( pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x );
    m_fCameraPitchAngle = -atan2f( pZBasis->y, fLen );
}
};