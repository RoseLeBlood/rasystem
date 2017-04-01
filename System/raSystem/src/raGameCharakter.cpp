#include "..\include\raMain.h"

namespace System
{
		raGameCharakter::raGameCharakter(raGame* pGame,
			UINT team,
			raVector3 position, raVector3 destination,
			raVector3 scale) : m_Position(position)
	{
		m_pGame = pGame;

		D3DXMatrixScaling((D3DXMATRIX*)&m_scale, scale.x, scale.y, scale.z);
		m_speed = 1.0f;

		//m_pGroupNode = pGroupNode;
		D3DXMATRIX m;
		D3DXMatrixIdentity(&m);
		//m_InstanceNo = m_pGroupNode->AddInstance(m);
		SetDestination(destination);

		m_Team = team;
		m_pFollow = NULL;
		m_bIsFighting = false;
		m_MaxHealth = 10;
		m_AktHealth = 10;
	}

	raGameCharakter::~raGameCharakter()
	{
		//m_pGroupNode->RemoveInstance(m_InstanceNo);
	}

	void raGameCharakter::SetDestination(const raVector3& destination)
	{
		if(m_AktHealth <= 0) return;

		m_destination = destination;
		raVector3 direction = m_destination - m_Position;
		D3DXVec3Normalize((D3DXVECTOR3*)&direction, (D3DXVECTOR3*)&direction);
		m_velocity = m_speed * direction;

		m_bIsFighting = false;

		SetAnimation(0);
	}

	void raGameCharakter::Update(double fTime, float fElapsedTime)
	{
		if(m_AktHealth <= 0) return;

		if(m_pFollow)
		{
			m_destination = m_pFollow->GetPosition();
		}

		raVector3 direction = m_destination - m_Position;
		if(D3DXVec3Length((D3DXVECTOR3*)&direction) <= m_speed)
		{
			m_velocity.x = m_velocity.y=m_velocity.z = 0.0f;
			if(m_pFollow && m_pFollow->m_Team != m_Team) //Gegner angreifen
			{
				m_bIsFighting = true;
				SetAnimation(4);

				static float t = 0;
				t += fElapsedTime;
				if(t > 1)
				{
				  m_pFollow->TakeHit(this);
				  t = 0;
				}
			}
			else
			{
				m_pFollow = NULL;
				SetAnimation(99);
			}
			return;
		}
		else if(m_bIsFighting) //wieder verfolgen
		{
			SetAnimation(0);
		}

		D3DXVec3Normalize((D3DXVECTOR3*)&direction, (D3DXVECTOR3*)&direction);
		m_velocity = m_speed * direction;

		//Character soll zum Ziel schauen
		D3DXMATRIX invView;
		raVector3 up(0, 1, 0);
		D3DXMATRIX m;
		D3DXMatrixLookAtRH((D3DXMATRIX *)&m, (D3DXVECTOR3*)&m_Position, (D3DXVECTOR3*)&m_destination, (D3DXVECTOR3*)&up); //aber warum RH??
		D3DXMatrixInverse(&invView, NULL, &m);
		//m_pGroupNode->SetWorldMatrix(m_scale * invView, m_InstanceNo);

		m_Position += m_velocity * fElapsedTime;
	}

	void raGameCharakter::TakeHit(raGameCharakter* pOther)
	{
		m_AktHealth -= 1;
		if(m_AktHealth > m_MaxHealth / 2) //zurückhauen
		{
			m_pFollow = pOther;
		}
		else //weglaufen
		{
			raVector3 direction = pOther->GetPosition() -  m_Position;
			if(D3DXVec3Length((D3DXVECTOR3*)&direction) == 0) direction.x = 1;
			D3DXVec3Normalize((D3DXVECTOR3*)&direction, (D3DXVECTOR3*)&direction);
			SetDestination(10 * direction);
		}

		if(m_AktHealth <= 0)
		{
			//sterben
			SetFollow(NULL);
			m_speed = 0;

			SetAnimation(5);

			pOther->SetFollow(NULL);
			//m_pGame->RemoveCharacter(this);
		}
	}

	void raGameCharakter::SetSelected(bool bSelected)
	{
		//m_pGroupNode->SetSelected(bSelected);
	}

	void raGameCharakter::SetFollow(raGameCharakter* pOther)
	{
		if(m_AktHealth <= 0) return;

		m_bIsFighting = false;
		m_pFollow = pOther;

		SetAnimation(0);
	}

	void raGameCharakter::SetAnimation(UINT clip)
	{
		//GetVisual()->SetAnimation(clip, m_InstanceNo);
	}
}