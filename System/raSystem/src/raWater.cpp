#include "..\include\raMain.h"

namespace System
{
	raWater::raWater(raSmartPointer<raDirectX> dx,  int width, int depth, raMaterial* pMaterial):
	raVisual(dx)
	{
		m_DrawOder = 2;

		m_numMaterials = 1;
		m_pMaterials = new raMaterial*[1];
		m_pMaterials[0] = pMaterial;

		//Keine Subsets

		m_width = width;
		m_depth = depth;

		m_nVertices = m_width * m_depth;
		m_nIndices  = 6 * (m_width - 1) * (m_depth - 1);

		m_curBuffNo = 0;

		for(int i = 0; i < 3;i++)
		{
			m_pVertices[i] = NULL;
		}

		m_pNormals = NULL;
		m_pTexcoords = NULL;

		m_pIndices = NULL;
		m_pIB = NULL;
		for(int n = 0; n < 3; n++)
		{
			m_pVB[n] = NULL;
		}

		m_lastTimeStamp = 0;
		m_lastAnimationTimeStamp = 0;
		m_lastFrameTime = 0;
	}

	raWater::~raWater(void)
	{
		for(int i = 0; i < 3; i++)
		{
			SAFE_DELETE_ARRAY(m_pVertices[i]);
		}
		SAFE_DELETE_ARRAY(m_pNormals);
		SAFE_DELETE_ARRAY(m_pIndices);
	}

	bool raWater::CreateVertexLayout()
	{
		//Postion, Normale und Texcoords jetzt jeweils aus einem anderen Slot
		//Die Offsets sind alle 0
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
				0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
				1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
				2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		UINT numElements = 3;

		// Create the input layout
		D3DX11_PASS_DESC PassDesc;
		ID3DX11EffectTechnique* pTechnique = m_pMaterials[0]->GetEffectTechnique();
		pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
		m_dx->GetDevice()->CreateInputLayout(layout, numElements,
			PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize,
			&m_pVertexLayout);

		return true;
	}

	void raWater::CreateVertexBuffer()
	{
		for(int i = 0; i < 3; i++)
		{
			m_pVertices[i] =
				new raVector3[m_nVertices];
		}
		m_pNormals =
			new raVector3[m_nVertices];
		m_pTexcoords =
			new raVector2[m_nVertices];

		SetupVertices();

		D3D11_BUFFER_DESC bufferDesc[3];
		D3D11_SUBRESOURCE_DATA InitData[3];

		for(int n = 0; n < 3; n++)
		{
			bufferDesc[n].Usage = D3D11_USAGE_DEFAULT;
			bufferDesc[n].BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc[n].CPUAccessFlags = 0;
			bufferDesc[n].MiscFlags = 0;

			InitData[n].SysMemPitch = 0;
			InitData[n].SysMemSlicePitch = 0;
		}

		bufferDesc[0].ByteWidth = m_nVertices * sizeof(D3DXVECTOR3);
		InitData[0].pSysMem = m_pVertices[0];
		m_dx->GetDevice()->CreateBuffer(&bufferDesc[0], &InitData[0], &m_pVB[0]);

		bufferDesc[1].ByteWidth = m_nVertices * sizeof(D3DXVECTOR3);
		InitData[1].pSysMem = m_pNormals;
		m_dx->GetDevice()->CreateBuffer(&bufferDesc[1], &InitData[1], &m_pVB[1]);

		bufferDesc[2].ByteWidth = m_nVertices * sizeof(D3DXVECTOR2);
		InitData[2].pSysMem = m_pTexcoords;
		m_dx->GetDevice()->CreateBuffer(&bufferDesc[2], &InitData[2], &m_pVB[2]);

		// m_pVertices und m_pNormals werden noch gebraucht
		SAFE_DELETE_ARRAY(m_pTexcoords);
	}

	void raWater::SetupVertices()
	{
		int n = 0;
		for (int z = 0; z < m_depth; z++)
		{
			for (int x = 0; x < m_width; x++)
			{
				for (int b = 0; b < 3; b++)
				{
					m_pVertices[b][n].x = (float) x - m_width / 2;
					m_pVertices[b][n].y = 0.0f;
					m_pVertices[b][n].z = (float) z - m_depth / 2;
				}
				m_pTexcoords[n].x = x / (m_width - 1.0f);
				m_pTexcoords[n].y = z / (m_depth - 1.0f);
				//m_pTexcoords[n].y = 1.0f - z / (m_depth - 1.0f);

				m_pNormals[n].x = 0.0f;
				m_pNormals[n].y = 1.0f;
				m_pNormals[n].z = 0.0f;

				n+=1;
			}
		}
	}

	void raWater::CreateIndexBuffer()
	{
		SetupIndices();

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.ByteWidth = m_nIndices * sizeof(UINT16);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = m_pIndices;
		m_dx->GetDevice()->CreateBuffer( &bufferDesc, &InitData, &m_pIB);
	}

	void raWater::SetupIndices()
	{
		m_pIndices = new UINT16[m_nIndices];

		int n = 0;
		for(int z = 0; z < m_depth - 1; z++)
		{
			for(int x = 0; x < m_width - 1; x++)
			{
				m_pIndices[n + 0] = (UINT16) (z * m_width + x);
				m_pIndices[n + 1] = (UINT16) ((z + 1) * m_width + x);
				m_pIndices[n + 2] = (UINT16) (z * m_width + (x + 1));
				m_pIndices[n + 3] = (UINT16) ((z + 1) * m_width + x);
				m_pIndices[n + 4] = (UINT16) ((z + 1) * m_width + (x + 1));
				m_pIndices[n + 5] = (UINT16) (z * m_width + (x + 1));

				n+=6;
			}
		}
	}

	bool raWater::RenderMesh(LPCSTR techniqueName)
	{
		raMaterial* pMaterial = m_pMaterials[0];
		ID3DX11EffectTechnique* pTechnique = pMaterial->GetEffectTechnique(techniqueName);

		pMaterial->Setup();

		m_dx->GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		UINT Strides[3];
		UINT Offsets[3];

		Offsets[0] = 0;
		Offsets[1] = 0;
		Offsets[2] = 0;

		Strides[0] = sizeof(D3DXVECTOR3);
		Strides[1] = sizeof(D3DXVECTOR3);
		Strides[2] = sizeof(D3DXVECTOR2);

		m_dx->GetImmediateContext()->IASetVertexBuffers(0, 3, m_pVB, Strides, Offsets);

		m_dx->GetImmediateContext()->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R16_UINT, 0);

		D3DX11_TECHNIQUE_DESC techDesc;
		pTechnique->GetDesc( &techDesc );

		for(UINT p = 0; p < techDesc.Passes; ++p )
		{
			pTechnique->GetPassByIndex(p)->Apply(0, m_dx->GetImmediateContext());
			m_dx->GetImmediateContext()->DrawIndexed(m_nIndices, 0, 0);
		}
		return true;
	}

	bool raWater::Update(float fTime, float fElapsedTime)
	{
		const float C = 0.3f; // ripple speed
		const float D = 0.4f; // distance
		const float U = 0.05f; // viscosity
		const float T = 0.13f; // time

		m_lastFrameTime = fElapsedTime;
		m_lastTimeStamp += fElapsedTime;

		while (m_lastAnimationTimeStamp <= m_lastTimeStamp)
		{
				m_curBuffNo = (m_curBuffNo + 1) % 3;

				double TERM1 = (4.0f - 8.0f * C * C * T * T / (D * D)) / (U * T + 2);
				double TERM2 = (U * T - 2.0f) / (U * T + 2.0f);
				double TERM3 = (2.0f * C * C * T * T / (D * D)) / (U * T + 2);
				for(int z = 1; z < m_depth - 1; z++)
				{
					// Randwerte bleiben unbeeinflusst
					int row = z * m_width;
					int rowup = (z - 1) * m_width;
					int rowdown = (z + 1) * m_width;
					for(int x = 1; x < m_width - 1; x++)
					{
						m_pVertices[m_curBuffNo][row + x].y = (float)(
							TERM1 * (float)m_pVertices[(m_curBuffNo + 2) % 3][row+x].y
							+ TERM2 * (float)m_pVertices[(m_curBuffNo + 1) % 3][row+x].y
							+ TERM3 * ((float)m_pVertices[(m_curBuffNo + 2) % 3][row+x-1].y
								+ (float)m_pVertices[(m_curBuffNo + 2) % 3][row+x+1].y
								+ (float)m_pVertices[(m_curBuffNo + 2) % 3][rowup+x].y
								+ (float)m_pVertices[(m_curBuffNo + 2) % 3][rowdown+x].y));
					}
				}

				m_lastAnimationTimeStamp += (1.0f / ANIMATIONS_PER_SECOND);
		}

		CalculateNormals();

		m_dx->GetImmediateContext()->UpdateSubresource(m_pVB[0], 0, NULL,
			m_pVertices[m_curBuffNo], 0, 0);
		m_dx->GetImmediateContext()->UpdateSubresource(m_pVB[1], 0, NULL,
			m_pNormals, 0, 0);

		return true;
	}

	void raWater::CalculateNormals()
	{
		int x, z;
		raVector3* buf = m_pVertices[m_curBuffNo];

		D3DXVECTOR3* pNormals = new D3DXVECTOR3[m_nVertices];
		for(UINT n = 0; n < m_nVertices; n++)
		{
			pNormals[n] = D3DXVECTOR3(0, 0, 0);
		}

		int n = 0;
		for(UINT i = 0; i < m_nIndices / 3; i++)
		{
			UINT16 i0 = m_pIndices[n++];
			UINT16 i1 = m_pIndices[n++];
			UINT16 i2 = m_pIndices[n++];
			D3DXVECTOR3 v0 = buf[i0];
			D3DXVECTOR3 v1 = buf[i1];
			D3DXVECTOR3 v2 = buf[i2];
			D3DXVECTOR3 diff1 = v2 - v1 ;
			D3DXVECTOR3 diff2 = v0 - v1 ;
			D3DXVECTOR3 fn;
			D3DXVec3Cross(&fn, &diff1, &diff2);
			pNormals[(int)i0] += fn ;
			pNormals[(int)i1] += fn;
			pNormals[(int)i2] += fn;
		}

		n = 0;
		for(z = 0;z < m_depth;z++)
		{
			for(x = 0;x < m_width;x++)
			{
				D3DXVec3Normalize((D3DXVECTOR3*)&m_pNormals[n], &pNormals[n]);
				n += 1;
			}
		}
		delete[] pNormals;
	}

	void raWater::push(float x, float y, float depth)
	{
			x += m_width /2;
			y += m_depth /2;
			depth = depth * m_lastFrameTime * ANIMATIONS_PER_SECOND ;
			_PREP(depth, x, y, 0, 0);
			_PREP(depth, x, y, 0, 1);
			_PREP(depth, x, y, 1, 0);
			_PREP(depth, x, y, 1, 1);
	}

	void raWater::pushX(float depth)
	{
			for (int x = 0; x < m_width; x++)
			{
				_PREP(depth, (float)x, 1, 0, 0);
			}
	}

	void raWater::pushZ(float depth)
	{
			for (int z = 0; z < m_depth; z++)
			{
				_PREP(raGetRandFloat(-abs(depth), abs(depth)), 1, (float) z, 0, 0);
				//_PREP(depth * sin (4 * 2 * D3DX_PI * z / (m_depth - 1)), 1, (float)z, 0, 0);
			}
	}

	void raWater::_PREP(float depth, float x, float z, int addx, int addz)
	{
		int vertex = (int)(z + addz) * m_width + (int)(x + addx);
		float diffz = (float)(z - floor(z + addz));
		float diffx = (float)(x - floor(x + addx));
		float dist = (float)(sqrt(diffz * diffz + diffx * diffx));
		float power = 1 - dist;
		if (power < 0) power = 0;
		m_pVertices[m_curBuffNo][vertex].y += depth * power;
	}
};