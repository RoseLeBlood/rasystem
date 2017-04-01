#include "..\include\raMain.h"

namespace System
{
	raTriangle::raTriangle(
		raSmartPointer<raDirectX> dx) :
		raEntity(dx, new raMaterial(dx.get(),
		"raEffects\\MyEngine.fx", "RenderWithColor"))
	{
	   m_nVertices = 3;
	   m_nIndices = 0; //unbenutzt
	}

	void raTriangle::SetupVertices()
	{
		m_pVertices =  new VERTEXPOSITIONCOLOR[m_nVertices];

		m_pVertices[0].position.x = -1.0f;
		m_pVertices[0].position.y = 0.0f;
		m_pVertices[0].position.z = 0.0f;

		m_pVertices[0].color[0] = 1.0f;
		m_pVertices[0].color[1] = 0.0f;
		m_pVertices[0].color[2] = 0.0f;

		m_pVertices[1].position.x = 0.0f;
		m_pVertices[1].position.y = 1.0f;
		m_pVertices[1].position.z = 0.0f;

		m_pVertices[1].color[0] = 0.0f;
		m_pVertices[1].color[1] = 1.0f;
		m_pVertices[1].color[2] = 0.0f;

		m_pVertices[2].position.x = 1.0f;
		m_pVertices[2].position.y = 0.0f;
		m_pVertices[2].position.z = 0.0f;

		m_pVertices[2].color[0] = 0.0f;
		m_pVertices[2].color[1] = 0.0f;
		m_pVertices[2].color[2] = 1.0f;
	}
	raQuad::raQuad(raSmartPointer<raDirectX> dx) :
		raEntity(dx, new raMaterial(dx.get(),
		 "raEffects\\MyEngine.fx", "RenderWithColor"))
	{
	   m_nVertices = 4;
	   m_nIndices = 6;
	}

	raQuad::raQuad(raSmartPointer<raDirectX> dx,   raMaterial *praMaterial) :
	raEntity(dx, praMaterial)
	{
	   m_nVertices = 4;
	   m_nIndices = 6;
	}

	void raQuad::SetupVertices()
	{
		m_pVertices =  new VERTEXPOSITIONCOLOR[m_nVertices];

		m_pVertices[0].position.x = -1.0f;
		m_pVertices[0].position.y = -1.0f;
		m_pVertices[0].position.z = 0.0f;

		m_pVertices[0].color[0] = 1.0f;
		m_pVertices[0].color[1] = 0.0f;
		m_pVertices[0].color[2] = 0.0f;

		m_pVertices[1].position.x = -1.0f;
		m_pVertices[1].position.y = 1.0f;
		m_pVertices[1].position.z = 0.0f;

		m_pVertices[1].color[0] = 0.0f;
		m_pVertices[1].color[1] = 1.0f;
		m_pVertices[1].color[2] = 0.0f;

		m_pVertices[2].position.x = 1.0f;
		m_pVertices[2].position.y = -1.0f;
		m_pVertices[2].position.z = 0.0f;

		m_pVertices[2].color[0] = 0.0f;
		m_pVertices[2].color[1] = 0.0f;
		m_pVertices[2].color[2] = 1.0f;

		m_pVertices[3].position.x = 1.0f;
		m_pVertices[3].position.y = 1.0f;
		m_pVertices[3].position.z = 0.0f;

		m_pVertices[3].color[0] = 0.0f;
		m_pVertices[3].color[1] = 0.0f;
		m_pVertices[3].color[2] = 0.0f;
	}

	void raQuad::SetupIndices()
	{
		m_pIndices = new UINT16[m_nIndices];

		m_pIndices[0] = 0;
		m_pIndices[1] = 1;
		m_pIndices[2] = 2;
		m_pIndices[3] = 1;
		m_pIndices[4] = 3;
		m_pIndices[5] = 2;

		m_pSubsets[0].IndexStart = 0;
		m_pSubsets[0].IndexCount = m_nIndices;
	}

	raCube::raCube(raSmartPointer<raDirectX> dx) :
		raEntity(dx, new raMaterial(dx.get(),
		 "raEffects\\MyEngine.fx", "RenderWithColor"))
	{
	   m_nVertices = 8;
	   m_nIndices = 36;
	}

	void raCube::SetupVertices()
	{
		m_pVertices =  new VERTEXPOSITIONCOLOR[m_nVertices];

		m_pVertices[0].position.x = -1.0f;
		m_pVertices[0].position.y = -1.0f;
		m_pVertices[0].position.z = -1.0f;

		m_pVertices[0].color[0] = 0.0f;
		m_pVertices[0].color[1] = 0.0f;
		m_pVertices[0].color[2] = 0.0f;

		m_pVertices[1].position.x = -1.0f;
		m_pVertices[1].position.y = 1.0f;
		m_pVertices[1].position.z = -1.0f;

		m_pVertices[1].color[0] = 0.0f;
		m_pVertices[1].color[1] = 0.0f;
		m_pVertices[1].color[2] = 1.0f;

		m_pVertices[2].position.x = 1.0f;
		m_pVertices[2].position.y = -1.0f;
		m_pVertices[2].position.z = -1.0f;

		m_pVertices[2].color[0] = 0.0f;
		m_pVertices[2].color[1] = 1.0f;
		m_pVertices[2].color[2] = 0.0f;

		m_pVertices[3].position.x = 1.0f;
		m_pVertices[3].position.y = 1.0f;
		m_pVertices[3].position.z = -1.0f;

		m_pVertices[3].color[0] = 0.0f;
		m_pVertices[3].color[1] = 1.0f;
		m_pVertices[3].color[2] = 1.0f;

		m_pVertices[4].position.x = -1.0f;
		m_pVertices[4].position.y = -1.0f;
		m_pVertices[4].position.z = 1.0f;

		m_pVertices[4].color[0] = 1.0f;
		m_pVertices[4].color[1] = 0.0f;
		m_pVertices[4].color[2] = 0.0f;

		m_pVertices[5].position.x = -1.0f;
		m_pVertices[5].position.y = 1.0f;
		m_pVertices[5].position.z = 1.0f;

		m_pVertices[5].color[0] = 1.0f;
		m_pVertices[5].color[1] = 0.0f;
		m_pVertices[5].color[2] = 1.0f;

		m_pVertices[6].position.x = 1.0f;
		m_pVertices[6].position.y = -1.0f;
		m_pVertices[6].position.z = 1.0f;

		m_pVertices[6].color[0] = 1.0f;
		m_pVertices[6].color[1] = 1.0f;
		m_pVertices[6].color[2] = 0.0f;

		m_pVertices[7].position.x = 1.0f;
		m_pVertices[7].position.y = 1.0f;
		m_pVertices[7].position.z = 1.0f;

		m_pVertices[7].color[0] = 1.0f;
		m_pVertices[7].color[1] = 1.0f;
		m_pVertices[7].color[2] = 1.0f;
	}

	void raCube::SetupIndices()
	{
		m_pIndices = new UINT16[m_nIndices];

		//vorn
		m_pIndices[0] = 0;
		m_pIndices[1] = 1;
		m_pIndices[2] = 2;
		m_pIndices[3] = 1;
		m_pIndices[4] = 3;
		m_pIndices[5] = 2;

		//hinten
		m_pIndices[6] = 4;
		m_pIndices[7] = 6;
		m_pIndices[8] = 5;
		m_pIndices[9] = 5;
		m_pIndices[10] = 6;
		m_pIndices[11] = 7;

		//links
		m_pIndices[12] = 4;
		m_pIndices[13] = 5;
		m_pIndices[14] = 1;
		m_pIndices[15] = 4;
		m_pIndices[16] = 1;
		m_pIndices[17] = 0;

		//rechts
		m_pIndices[18] = 2;
		m_pIndices[19] = 3;
		m_pIndices[20] = 7;
		m_pIndices[21] = 2;
		m_pIndices[22] = 7;
		m_pIndices[23] = 6;

		//unten
		m_pIndices[24] = 0;
		m_pIndices[25] = 6;
		m_pIndices[26] = 4;
		m_pIndices[27] = 0;
		m_pIndices[28] = 2;
		m_pIndices[29] = 6;

		//oben
		m_pIndices[30] = 1;
		m_pIndices[31] = 5;
		m_pIndices[32] = 7;
		m_pIndices[33] = 1;
		m_pIndices[34] = 7;
		m_pIndices[35] = 3;

		m_pSubsets[0].IndexStart = 0;
		m_pSubsets[0].IndexCount = m_nIndices;
	}

	raSphere::raSphere(raSmartPointer<raDirectX> dx, int width, int height,
		float radius, raMaterial* praMaterial):
		raEntity(dx, praMaterial ? praMaterial :
		new raTexturedMaterial(dx,
		"raEffects\\MyEngine.fx", "RenderScene",
		"raGrafik\\earth.bmp"))
	{
	   m_r = radius;
	   m_width = width;
	   m_height = height;

	   m_nVertices = m_width * m_height;
	   m_nIndices  = 6 * (m_width - 1) * (m_height - 1);
	}

	void raSphere::SetupVertices()
	{
		m_pVertices =
			new VERTEXPOSITIONNORMALTEXTURED[m_nVertices];

		int n = 0;

		for (int y = 0; y < m_height; y++)
		{
			float theta;
			//Polkappen öffnen
			if(y == 0)
				theta = PI * (0.5f / ((float)m_height - 1));
			else if(y == m_height - 1)
					theta = PI * ((m_height - 1.5f) / ((float)m_height - 1));
			else
					theta = PI * (y / ((float)m_height - 1));
			float rsintheta = m_r * sin(theta);
			float rcostheta = m_r * cos(theta);
			for (int x = 0; x < m_width; x++)
			{
				float phi = 2 * PI * x / ((float)m_width - 1);
				m_pVertices[n].position.x = rsintheta * cos(phi);
				m_pVertices[n].position.y = rcostheta;
				m_pVertices[n].position.z = rsintheta * sin(phi);

				m_pVertices[n].texcoord.x = x / ((float)m_width - 1);
				m_pVertices[n].texcoord.y = y / ((float)m_height - 1);
				m_pVertices[n].normal = m_pVertices[n].position;
				n++;
			}
		}
	}

	void raSphere::SetupIndices()
	{
		m_pIndices = new UINT16[m_nIndices];
		int n = 0;
		for(int y = 0; y < m_height - 1; y++)
		{
			for(int x = 0; x < m_width - 1; x++)
			{
				//Winding Order umgekehrt
				m_pIndices[n + 0] = (UINT16) (y * m_width + x);
				m_pIndices[n + 2] = (UINT16) ((y + 1) * m_width + x);
				m_pIndices[n + 1] = (UINT16) (y * m_width + (x + 1));
				m_pIndices[n + 3] = (UINT16) ((y + 1) * m_width + x);
				m_pIndices[n + 5] = (UINT16) ((y + 1) * m_width + (x + 1));
				m_pIndices[n + 4] = (UINT16) (y * m_width + (x + 1));

				n+=6;
			}
		}

		m_pSubsets[0].IndexStart = 0;
		m_pSubsets[0].IndexCount = m_nIndices;
	}

	raRotationskoerper::raRotationskoerper(raSmartPointer<raDirectX> dx,   int height)
		: raEntity(dx, new raMaterial(dx.get(),
		 "raEffects\\MyEngine.fx", "RenderWithColor"))
	{
	   m_height = height;
	   m_nSlices = 36;

	   m_nVertices = m_nSlices * m_height;
	   m_nIndices = 6 * m_nSlices * (m_height - 1);// nicht m_nSlices - 1, weil die Naht geschlossen wird
	}

	void raRotationskoerper::SetupVertices()
	{
		m_pVertices =
			new VERTEXPOSITIONCOLOR[m_nVertices];

		int n = 0;
		for(int y = 0; y < m_height; y++)
		{
			for(int phi = 0; phi < m_nSlices; phi++)
			{
				float r0 = r(y);
				float phi0 = 2 * PI * phi / (m_nSlices-1);
				m_pVertices[n].position.x = (float) r0 * sin(phi0);
				m_pVertices[n].position.y = (float) y;
				m_pVertices[n].position.z = (float) r0 * cos(phi0);

				m_pVertices[n].color[0] = (float) rand() / RAND_MAX; //1.0f;
				m_pVertices[n].color[1] = (float) rand() / RAND_MAX; //1.0f;
				m_pVertices[n].color[2] = (float) rand() / RAND_MAX; //1.0f;
				n+=1;
			}
		}
	}

	void raRotationskoerper::SetupIndices()
	{
	   m_pIndices = new UINT16[m_nIndices];

	   int n = 0;
	   for(int y = 0; y < m_height - 1; y++)
	   {
		  for(int phi = 0; phi < m_nSlices; phi++)
		  {
			 m_pIndices[n + 0] = (UINT16)(y * m_nSlices + phi);
			 m_pIndices[n + 1] = (UINT16)(y * m_nSlices + (phi + 1)%m_nSlices);
			 m_pIndices[n + 2] = (UINT16)((y+1) * m_nSlices + phi);
			 m_pIndices[n + 3] = (UINT16)((y+1) * m_nSlices + phi);
			 m_pIndices[n + 4] = (UINT16)(y * m_nSlices + (phi + 1)%m_nSlices);
			 m_pIndices[n + 5] = (UINT16)((y+1) * m_nSlices + (phi + 1)%m_nSlices);

			 n+=6;
		  }
	   }

	   m_pSubsets[0].IndexStart = 0;
	   m_pSubsets[0].IndexCount = m_nIndices;
	}

	raLineList::raLineList(raSmartPointer<raDirectX> dx, int level) :
	raEntity(dx, new raMaterial(dx.get(),
		"raEffects\\MyEngine.fx", "RenderWithColor"))
	{
		m_level = level;
		m_nVertices = 0;
		for(int i = 1; i < level + 3; i++)
			m_nVertices += (UINT16) pow(2.0, i);
		m_nIndices = 0; //unbenutzt
	}

	void raLineList::makeChildren(VERTEXPOSITIONCOLOR* m_pVertices, D3DXVECTOR3 v0, D3DXVECTOR3 d, int& n, int level)
	{
		float r = (float) rand() / RAND_MAX;
		float b = (float) rand() / RAND_MAX;

		//Anfangs- und Endpunkt hinzufügen
		m_pVertices[n].position.x = v0.x;
		m_pVertices[n].position.y = v0.y;
		m_pVertices[n].position.z = v0.z;
		m_pVertices[n].color[0] = r;
		m_pVertices[n].color[1] = 1.0f;
		m_pVertices[n].color[2] = b;
		n += 1;

		m_pVertices[n].position.x = v0.x + d.x;
		m_pVertices[n].position.y = v0.y + d.y;
		m_pVertices[n].position.z = v0.z + d.z;
		m_pVertices[n].color[0] = r;
		m_pVertices[n].color[1] = 1.0f;
		m_pVertices[n].color[2] = b;
		n += 1;

		 //Zwei Kinder
		 if(level >= 0)
		 {
			 D3DXVECTOR4 v2, v3;

			 D3DXMATRIX matLeftRot;
			 D3DXMatrixRotationZ(&matLeftRot, PI / 16.0f);
			 D3DXVec3Transform(&v2, &d, &matLeftRot);
			 makeChildren(m_pVertices, v0 + d, 0.9f * (D3DXVECTOR3) v2, n, level - 1);

			 D3DXMATRIX matRightRot;
			 D3DXMatrixRotationZ(&matRightRot, -PI / 16.0f);
			 D3DXVec3Transform(&v3, &d, &matRightRot);
			 makeChildren(m_pVertices, v0 + d, 0.9f * (D3DXVECTOR3) v3, n, level - 1);
		 }
	}
	 void raLineList::SetupVertices()
	{
		m_pVertices =
			new VERTEXPOSITIONCOLOR[m_nVertices];

		int n = 0;
		D3DXVECTOR3 v0(0, 0, 0);
		D3DXVECTOR3 v1(0, 1, 0);
		makeChildren((VERTEXPOSITIONCOLOR*)m_pVertices, v0, v1, n, m_level);
	}

	 void raLineList::RenderMesh(ID3D11DeviceContext* pd3dImmediateContext,ID3DX11EffectTechnique* pTechnique)
	{
	}

	raTexturedQuad::raTexturedQuad(raSmartPointer<raDirectX> dx, raMaterial* praMaterial,
		bool bAlpha) : raEntity(dx, praMaterial, bAlpha)
	{
	   m_nVertices = 4;
	   m_nIndices = 6;
	  // m_DrawOrder = 3;
	   m_bDepthDisable = true;
	}

	void raTexturedQuad::SetupVertices()
	{
		m_pVertices =
			new VERTEXPOSITIONTEXTURED[m_nVertices];

		m_pVertices[0].position.x = -1.0f;
		m_pVertices[0].position.y = -1.0f;
		m_pVertices[0].position.z = 0.0f;

		m_pVertices[1].position.x = -1.0f;
		m_pVertices[1].position.y = 1.0f;
		m_pVertices[1].position.z = 0.0f;

		m_pVertices[2].position.x = 1.0f;
		m_pVertices[2].position.y = -1.0f;
		m_pVertices[2].position.z = 0.0f;

		m_pVertices[3].position.x = 1.0f;
		m_pVertices[3].position.y = 1.0f;
		m_pVertices[3].position.z = 0.0f;

		for(int i = 0; i < 4; i++)
		{
			m_pVertices[i].texcoord.x =   0.5f * (float)m_pVertices[i].position.x + 0.5f;
			m_pVertices[i].texcoord.y = - 0.5f * (float)m_pVertices[i].position.y - 0.5f;
		}
	}

	void raTexturedQuad::SetupIndices()
	{
		m_pIndices = new UINT16[m_nIndices];

		m_pIndices[0] = 0;
		m_pIndices[1] = 1;
		m_pIndices[2] = 2;
		m_pIndices[3] = 1;
		m_pIndices[4] = 3;
		m_pIndices[5] = 2;

		m_pSubsets[0].IndexStart = 0;
		m_pSubsets[0].IndexCount = m_nIndices;
	}

	raAchsenkreuz::raAchsenkreuz(raSmartPointer<raDirectX> dx) :
	raEntity(dx, new raMaterial(dx,
		 "raEffects\\MyEngine.fx", "RenderWithColor"))
	{
	   m_nVertices = 6 + 3 * 20;
	   m_nIndices = 0; //unbenutzt
	   m_pSubsets[0].PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	}

	void raAchsenkreuz::SetupVertices()
	{
		m_pVertices =
			new VERTEXPOSITIONCOLOR[m_nVertices];

		m_nVertices = 0;
		for (int axis = 0; axis < 3; axis++)
		{
			for (int n = 0; n <= 1; n++)
			{
				m_pVertices[m_nVertices].position.x = (float) (axis == 0? n: 0);
				m_pVertices[m_nVertices].position.y = (float) (axis == 1? n: 0);
				m_pVertices[m_nVertices].position.z = (float) (axis == 2? n: 0);

				m_pVertices[m_nVertices].color[axis] = 1;
				m_pVertices[m_nVertices].color[(axis + 1) % 3] = 0;
				m_pVertices[m_nVertices].color[(axis + 2) % 3] = 0;
				m_nVertices++;
			}

			//Ticks zeichnen
			for (int i = 0; i < 10; i++)
			{
				for (int j = -1; j <= 1; j+=2)
				{
					m_pVertices[m_nVertices].position.x =
						(axis == 0? 0.1f * i : (axis == 1? 0.01f * j : 0));
					m_pVertices[m_nVertices].position.y =
						(axis == 1? 0.1f * i : 0.01f * j);
					m_pVertices[m_nVertices].position.z =
						(axis == 2? 0.1f * i : 0);

					m_pVertices[m_nVertices].color[axis] = 1;
					m_pVertices[m_nVertices].color[(axis + 1) % 3] = 0;
					m_pVertices[m_nVertices].color[(axis + 2) % 3] = 0;
					m_nVertices++;
				}
			}
		}
	}
};