#include "..\include\raMain.h"
namespace System
{
	raPhysik::raPhysik(raRender *rand) :
		m_Velocity(0,0,0), m_Accelerate(0,0,0), m_AngularAccelerate(0,0,0), m_AngularVelocity(0,0,0)
	{
		m_pRender = rand;

		m_Translation = raMatrixIdentity();
		m_Rotation = raMatrixIdentity();

		m_Scaling = raMatrixScaling(raVector3(m_pRender->GetWorldMatrix()->m11,
											  m_pRender->GetWorldMatrix()->m22,
											  m_pRender->GetWorldMatrix()->m33));

		m_Mass = 1;
	}
	raPhysik::raPhysik(raRender* rend, const raVector3& Position, const raVector3& Velocity,
					   const raVector3& Accelerator, float mass) :
		m_Velocity(Velocity), m_Accelerate(Accelerator), m_AngularVelocity(0,0,0), m_AngularAccelerate(0,0,0)
	{
		m_pRender = rend;

		D3DXMatrixTranslation((D3DXMATRIX*)&m_Translation, Position.x, Position.y, Position.z);
		m_Rotation = raMatrixIdentity();

		m_Scaling = raMatrixScaling(raVector3(m_pRender->GetWorldMatrix()->m11,
											  m_pRender->GetWorldMatrix()->m22,
											  m_pRender->GetWorldMatrix()->m33));

		m_Mass = mass;
	}
//-----------------------------------------------------------------------------------------------

	raPhysik::~raPhysik(void)
	{
	}
	void raPhysik::Update(float fTime, float fRunTime)
	{
		raVector3 translation;
		D3DXVec3Scale((D3DXVECTOR3*)translation, (D3DXVECTOR3*)m_Velocity, fTime);

		float angle = fTime * raVector3LenghtSq(m_AngularAccelerate);
		raVector3 axis;
		D3DXVec3Normalize((D3DXVECTOR3*)&axis, (D3DXVECTOR3*)&m_AngularVelocity);

		if(m_pRender)
		{
			raMatrix mr, mt, mWorld;

			mr = raMatrixRotationAxis(axis, angle);
			m_Rotation *= mr;

			D3DXMatrixTranslation((D3DXMATRIX*)&mt, translation.x, translation.y, translation.z);
			m_Translation *= mt;

			raMatrix m;
			m = m_Scaling * m_Rotation;
			mWorld = m * m_Translation;

			m_pRender->SetWorldMatrix(mWorld);
		}
		m_Velocity += fTime * m_Accelerate;
		m_AngularVelocity += fTime * m_AngularAccelerate;
	}
	void raPhysik::SetAccelerate(float x, float y, float z)
	{
		SetAccelerate(raVector3(x, y, z));
	}
	void raPhysik::SetAccelerate(raVector3 Acc)
	{
		m_Accelerate = Acc;
	}
	void raPhysik::SetAngAccel(float x, float y, float z)
	{
		SetAngAccel(raVector3(x, y, z));
	}
	void raPhysik::SetAngAccel(raVector3 Ang)
	{
		m_AngularAccelerate = Ang;
	}
	void raPhysik::StopMotion()
	{
		m_Accelerate.x = 0;
		m_Accelerate.y = 0;
		m_Accelerate.z = 0;

		m_AngularAccelerate.x = 0;
		m_AngularAccelerate.y = 0;
		m_AngularAccelerate.z = 0;

		m_Velocity.x = 0;
		m_Velocity.y = 0;
		m_Velocity.z = 0;

		m_AngularVelocity.x = 0;
		m_AngularVelocity.y = 0;
		m_AngularVelocity.z = 0;
	}
	raVector3 raPhysik::GetPosition()
	{
		raVector3 pos1(0,0,0), pos2;
		D3DXVec3TransformCoord((D3DXVECTOR3*)&pos2, (D3DXVECTOR3*)&pos1, (D3DXMATRIX*)&m_Translation);

		return pos2;
	}
	raVector3 raPhysik::GetVelocity()
	{
		return m_Velocity;
	}
	float raPhysik::GetMass()
	{
		return m_Mass;
	}
	float raPhysik::GetBoundingSphereRadius()
	{
		return m_pRender->GetBoundingsphereRadius();
	}

	void raPhysik::SetVelocity(float x, float y, float z)
	{
		m_Velocity.x = x;
		m_Velocity.y = y;
		m_Velocity.z = z;
	}
	void raPhysik::Accelerate(float x, float y, float z)
	{
		raVector3 a(x, y, z);
		D3DXVec3TransformNormal((D3DXVECTOR3*)&m_Accelerate, (D3DXVECTOR3*)&a, (D3DXMATRIX*)m_pRender->GetWorldMatrix());
	}
	void raPhysik::AngAccel(float x, float y, float z)
	{
		raVector3 a(x, y, z);
		D3DXVec3TransformNormal((D3DXVECTOR3*)&m_AngularAccelerate, (D3DXVECTOR3*)&a, (D3DXMATRIX*)m_pRender->GetWorldMatrix());
	}
};