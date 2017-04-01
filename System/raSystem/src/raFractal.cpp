#include "..\include\raMain.h"

namespace System
{
	raFractal::raFractal(raSmartPointer<raDirectX> dx, int Level, bool bRandom, raMaterial* mat)
	: raEntity(dx, mat ? mat : new raMaterial(dx, "raEffects\\MyEngine.fx", "RenderWithColor"))
	{
		m_pSubsets[0].PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		m_level = Level;

		m_nVertices = 0;

		for(int i = 0; i < Level + 2; i++)
			m_nVertices += (UINT) pow(4.0, i + 1);
		m_nIndices = 0;

		m_pVB = NULL;
		m_max_angle = 45 * PI / 180.0f;
		m_max_lenght = 0.5f;
		m_bRandom = bRandom;
	}
	void raFractal::SetupVertices()
	{
		m_pVertices = new VERTEXPOSITIONCOLOR[m_nVertices];

		int n = 0;
		raVector3 v0(0.0f, 0.0f, 0.0f);
		raVector3 v1(0.0f, 1.0f, 0.0f);

		makeChildren((VERTEXPOSITIONCOLOR*)m_pVertices, v0, v1, n, m_level);
	}
	void  raFractal::makeChildren(VERTEXPOSITIONCOLOR* vertices,
			raVector3 v0, raVector3 d, int& n, int level)
	{
		float r = (float) rand() / RAND_MAX;
		float g = (float) rand() / RAND_MAX;
		float b = (float) rand() / RAND_MAX;

		//Anfangs- und Endpunkt hinzufügen
		vertices[n].position.x = v0.x;
		vertices[n].position.y = v0.y;
		vertices[n].position.z = v0.z;
		vertices[n].color[0] = r;
		vertices[n].color[1] = 1.0f;
		vertices[n].color[2] = b;

		n += 1;

		vertices[n].position.x = v0.x + d.x;
		vertices[n].position.y = v0.y + d.y;
		vertices[n].position.z = v0.z + d.z;
		vertices[n].color[0] = r;
		vertices[n].color[1] = 1.0f;
		vertices[n].color[2] = b;
		n += 1;

		// Vier Kinder
		float angle;
		if(m_bRandom)
			angle = m_max_angle * rand() / RAND_MAX;
		else
			angle = m_max_angle;

		if( level >= 0)
		{
			raVector4 v;
			raMatrix m[4];

			m[0] = raMatrixRotationZ( angle);
			m[1] = raMatrixRotationZ( -angle);
			m[2] = raMatrixRotationX( angle);
			m[3] = raMatrixRotationX( -angle);

			float l;

			for(int i = 0; i < 4; i++)
			{
				if(m_bRandom)
					l = 2.0f * m_max_lenght * (float) rand() / RAND_MAX;
				else
					l = m_max_lenght;

				D3DXVec3Transform((D3DXVECTOR4*)&v, (D3DXVECTOR3*)&d, (D3DXMATRIX*)&m[i]);
				D3DXVECTOR3 v1 = (D3DXVECTOR3)v;

				makeChildren(vertices, v0 + d, l * v1, n, level - 1);
			}
		}
	}
};