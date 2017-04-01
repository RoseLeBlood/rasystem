#pragma once

	class RAPI raGameCharakter
	{
	public:
		raGameCharakter(raGame* pGame,
						UINT team, raVector3 position, raVector3 destination,
						raVector3 scale = raVector3(1, 1, 1));

		raGameCharakter(const GUID& raguid, raGame* pGame,
						UINT team, raVector3 position, raVector3 destination,
						raVector3 scale = raVector3(1, 1, 1));

		~raGameCharakter(void);

		void Update(double fTime, float fElapsedTime);

		raVector3 GetPosition()		{ return m_Position; }
		UINT GetHealth()			{ return m_AktHealth; }

		void SetDestination(const raVector3& destination);
		void SetSelected(bool bSelected);
		void SetFollow(raGameCharakter* pOther);

		void TakeHit(raGameCharakter* pOther);

		void SetAnimation(UINT clip);
	protected:
		UINT				m_InstanceNo;
		UINT				m_Team;
		UINT				m_MaxHealth;
		UINT				m_AktHealth;

		raMatrix			m_scale;
		raVector3			m_Position;
		raVector3			m_velocity;
		raVector3			m_destination;

		float				m_speed;

		raGame*				m_pGame;
		raGameCharakter*	m_pFollow;
		bool				m_bIsFighting;
	};