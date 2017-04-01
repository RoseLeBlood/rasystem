#include "..\include\raMain.h"

namespace System
{
raLight::raLight(void) : m_Position(-10.0f, 2.0f, 0.0f),
						 m_Direction(1.0f, 0.0f, 0.0f),
						 m_DiffuseColor(0.9f),
						 m_AmbientColor(0.1f),
						 m_Attenuation(0, 0, 0.4f, 0)
{
	m_cosHalfPhi = 0.4f;
	m_cosHalfTheta = 0.9f;
}

raLight::~raLight(void)
{
}
raMatrix raLight::GetViewMatrix() const
{
	raVector3 vUp = raVector3(0.0f, 1.0f, 0.0f);
	raVector3 eye = m_Position;
	raVector3 lookAt = m_Position + m_Direction;

	raMatrix Out;
	D3DXMatrixLookAtLH((D3DXMATRIX*)&Out, (D3DXVECTOR3*)&eye, (D3DXVECTOR3*)&lookAt,
		(D3DXVECTOR3*)&vUp);

	return Out;
}
raMatrix raLight::GetProjectionMatrix() const
{
	return m_mProj;
}

void raLight::SetProjParams(float fFov, float fAspect, float fNearPlane, float fFarPlane)
{
	m_fFOV = fFov;
	m_fAspect = fAspect;
	m_fNearPlane = fNearPlane;
	m_fFarPlane = fFarPlane;

	D3DXMatrixPerspectiveFovLH((D3DXMATRIX*)&m_mProj, fFov, fAspect, fNearPlane, fFarPlane);
}
};