#pragma once

	class RAPI raPhysik
	{
	public:
		raPhysik(raRender *rand);
		raPhysik(raRender* rend,
				 const raVector3& Position,
				 const raVector3& Velocity,
				 const raVector3& Accelerator,
				 float mass = 1.0f);

		virtual ~raPhysik(void);

		virtual void Update(float fTime, float fRunTime);

		void Accelerate(float x, float y, float z);
		void AngAccel(float x, float y, float z);

		void SetAccelerate(float x, float y, float z);
		void SetAccelerate(raVector3 Acc);

		void SetAngAccel(float x, float y, float z);
		void SetAngAccel(raVector3 Ang);

		void StopMotion();

		raVector3	GetPosition();
		raVector3	GetVelocity();
		float		GetMass();
		float		GetBoundingSphereRadius();

		void SetVelocity(float x, float y, float z);
	private:
		raMatrix		m_Translation;
		raVector3		m_Velocity;
		raVector3		m_Accelerate;

		raMatrix		m_Rotation;
		raVector3		m_AngularVelocity;
		raVector3		m_AngularAccelerate;

		raMatrix		m_Scaling;
		float			m_Mass;

		raRender		*m_pRender;
	};