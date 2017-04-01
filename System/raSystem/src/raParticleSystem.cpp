#include "..\include\raMain.h"

namespace System
{
	raParticle::raParticle()
	{
		m_Position = 0.0f;
		m_Velocity = 0.0f;
		m_Acceleration = 0.0f;
		m_age = 0.0f;
	}
	void raParticle::Update(float fTime)
	{
		m_age += fTime;
		m_Position += m_Velocity * fTime * m_speed;
		m_Velocity += m_Acceleration * fTime * m_speed;
	}
	raParticleSystem::~raParticleSystem(void)
	{
	}

	raParticleSystem::raParticleSystem(raSmartPointer<raDirectX> dx, raTexturedMaterial* mat, bool bAdditive,
									   float radius, float gravity, float Speed, float max_age) : raEntity(dx, mat, true, bAdditive)
	{
		m_radius = radius;
		m_gravity = gravity;
		m_maxAge = max_age;
		m_speed	= Speed;

		m_DrawOder = 2;
		m_bDepthDisable = true;
		m_pSubsets[0].PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	}
	void raParticleSystem::SetupVertices()
	{
		if(m_pVB == NULL)
		{
			m_nVertices = MAXPARTICLES;
		}
		else
			m_nVertices = (DWORD)m_Particles.size();

		m_pVertices = new POINTSPRITEVERTEX[m_nVertices];

		POINTSPRITEVERTEX *pVertex = (POINTSPRITEVERTEX*)m_pVertices;
		std::list<raParticle>::iterator pParticle;

		for(pParticle=m_Particles.begin(); pParticle != m_Particles.end(); pParticle++)
		{
			pVertex->position = pParticle->m_Position;
			pVertex->color[0] = 1;
			pVertex->color[1] = 1;
			pVertex->color[2] = 1;

			pVertex->radius = m_radius;
			pParticle->m_speed = m_speed;

			pVertex++;
		}
	}
	bool raParticleSystem::Update(float fTime, float fRunTime)
	{
		std::list<raParticle>::iterator pParticle;
		for(pParticle = m_Particles.begin();
			pParticle != m_Particles.end();)
		{
			pParticle->Update(fTime);
			if((m_maxAge > 0) && (pParticle->m_age > m_maxAge))
			{
				pParticle->m_Acceleration = raVector3(0.0f);
				pParticle->m_Position = raVector3(0.0f);
				pParticle->m_age = 0.0f;
				pParticle = m_Particles.erase(pParticle);
			}
			else
			{
				pParticle++;
			}
		}
		delete[] m_pVertices;
		m_pVertices = NULL;

		SetupVertices();
		if(m_nVertices > 0)
		{
			D3D11_BOX updateBox;
			memset(&updateBox, 0, sizeof(D3D11_BOX));
			updateBox.right = updateBox.left + m_nVertices * sizeof(POINTSPRITEVERTEX);
			updateBox.bottom = 1;
			updateBox.back = 1;

			m_dx->GetImmediateContext()->UpdateSubresource(m_pVB, 0, &updateBox, m_pVertices,
				sizeof(VERTEXPOSITIONCOLOR), sizeof(VERTEXPOSITIONCOLOR));
		}
		return true;
	}
	bool raParticleSystem::RenderMesh(LPCSTR techniqueName)
	{
		if(m_Particles.size() > 0)
		{
			raEntity::RenderMesh(techniqueName);
		}
		return true;
	}
	void raParticleSystem::AddParticles(int n,
		float x, float y, float z)
	{
		if(m_Particles.size() + n > MAXPARTICLES) return;

		for(int i = 0; i < n; i++)
		{
			raParticle p;
			p.m_Position.x = x;
			p.m_Position.y = y;
			p.m_Position.z = z;

			p.m_Velocity.x = (((float)rand() / RAND_MAX) - 0.5f) * 0.1f;
			p.m_Velocity.y = 0.1f;
			p.m_Velocity.z = (((float)rand() / RAND_MAX) - 0.5f) * 0.1f;

			p.m_Acceleration.y = m_gravity;

			m_Particles.push_back(p);
		}
	}

	void raParticleSystem::AddParticle(float x, float y, float z,
		float vx, float vy, float vz)
	{
		if(m_Particles.size() + 1 > MAXPARTICLES) return;

		raParticle p;
		p.m_Position.x = x;
		p.m_Position.y = y;
		p.m_Position.z = z;

		p.m_Velocity.x = vx;
		p.m_Velocity.y = vy;
		p.m_Velocity.z = vz;

		p.m_Acceleration.y = m_gravity;

		m_Particles.push_back(p);
	}
};