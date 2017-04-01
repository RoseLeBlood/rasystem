#pragma once

	struct RAPI raParticle
	{
		raVector3		m_Position;
		raVector3		m_Velocity;
		raVector3		m_Acceleration;

		float			m_age;
		float			m_speed;

		raParticle();
		void Update(float fTime);
	};

	class RAPI raParticleSystem : public raEntity<POINTSPRITEVERTEX, false>
	{
	public:
		static const int MAXPARTICLES = 500000;
	public:
		raParticleSystem(raSmartPointer<raDirectX> dx, raTexturedMaterial* mat, bool bAdditive = false,
			float radius = 1.0f, float gravity = 1.0f, float Speed = 2.0f, float max_age = 0.0f);
		virtual ~raParticleSystem(void);

		void AddParticles(int n, float x = 0, float y = 0, float z = 0);
		void AddParticle(float x = 0, float y = 0, float z = 0,
						 float vx = 0, float vy = 0, float vz = 0);

		virtual bool RenderMesh(LPCSTR techniqueName = "");
		virtual bool Update(float fTime, float fRunTime);

	protected:
		virtual void SetupVertices();
	private:
		std::list<raParticle>	m_Particles;

		raArray<raParticle>		m_Particlesex;

		float					m_radius;
		float					m_gravity;
		float					m_maxAge;
		float					m_speed;

		ID3DX11EffectMatrixVariable*	m_pmInvView_ParticleSystem;
	};