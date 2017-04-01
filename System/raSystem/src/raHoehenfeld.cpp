#include "..\include\raMain.h"

namespace System
{
	raHoehenfeld::raHoehenfeld(raSmartPointer<raDirectX> dx,  int width, int depth,
		raMaterial* mat, float tileSize): raEntity(dx, mat)
	{
	   m_width = width;
	   m_depth = depth;

	   m_tileSize = tileSize;

	   m_nVertices = m_width * m_depth;
	   m_nIndices  = 6 * (m_width - 1) * (m_depth - 1);
	}
	raHoehenfeld::~raHoehenfeld(void)
	{
	}

	void raHoehenfeld::SetupVertices()
	{
		m_pVertices = new VERTEXPOSITIONNORMALTEXTURED[m_nVertices];

		int n = 0;
		for(int z = 0; z < m_depth; z++)
		{
			for(int x = 0; x < m_width; x++)
			{
				m_pVertices[n].position.x = (float)x / (m_width - 1);
				m_pVertices[n].position.y =
					h(((float)x / (m_width - 1)),
					  ((float)z / (m_depth - 1)));
				m_pVertices[n].position.z = (float)z / (m_depth - 1);

			   m_pVertices[n].normal = normal(
					((float)x / (m_width - 1)),
					((float)z / (m_depth - 1)));

				m_pVertices[n].texcoord.x = x / m_tileSize;
				m_pVertices[n].texcoord.y = z / m_tileSize;

				n+=1;
			}
		}
	}
	void raHoehenfeld::SetupIndices()
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
		m_pSubsets[0].IndexStart = 0;
		m_pSubsets[0].IndexCount = m_nIndices;
	}
	//----------------
	raHeightMap::raHeightMap(raSmartPointer<raDirectX> dx, int width, int depth, raMaterial *mat, float tileSize,
							 int PixelFormat,
							 LPCSTR heightTexture, LPCSTR bumpMapTexture) :
							 raHoehenfeld(dx, width, depth, mat, tileSize)
	{
		m_PixelFormat = PixelFormat;
		//Daten einlesen
		HANDLE hFile;
		LARGE_INTEGER FileSize;
		DWORD nByte;
		DWORD nBytesRead;

		hFile = CreateFile(heightTexture, FILE_READ_DATA, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

		GetFileSizeEx(hFile, &FileSize);
		nByte = FileSize.LowPart;
		m_pHeightMap = new BYTE[nByte];

		ReadFile(hFile, m_pHeightMap, nByte, &nBytesRead, NULL);
		CloseHandle(hFile);
		assert(nBytesRead > 0);

		// Normal
		hFile = CreateFile(bumpMapTexture, FILE_READ_DATA, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

		GetFileSizeEx(hFile, &FileSize);
		nByte = FileSize.LowPart;
		m_pBumpData = new BYTE[nByte];

		ReadFile(hFile, m_pBumpData, nByte, &nBytesRead, NULL);
		CloseHandle(hFile);
	}
	raHeightMap::~raHeightMap()
	{
		SAFE_DELETE(m_pHeightMap);
		SAFE_DELETE(m_pBumpData);
	}
	float raHeightMap::h(float x, float z)
	{
		int w = m_PixelFormat;
		x *= w;
		z *= w;
		if(x >= w)
			x = (float)w-1;
		if(z >= w)
			z = (float)w-1;

		return m_pHeightMap[3 * ((UINT)x * w + (UINT)z)] / 255.0f;
	}
	raVector3 raHeightMap::normal(float x, float z)
	{
		int w = m_PixelFormat;

		z *= w;
		z *= w;

		float r = m_pBumpData[3 * ((UINT)x * w + (UINT)z) + 0];
		float g = m_pBumpData[3 * ((UINT)x * w + (UINT)z) + 1];
		float b = m_pBumpData[3 * ((UINT)x * w + (UINT)z) + 2];

		raVector3 n(r, g, b);
		n = raVector3Normalize(n);
		return n;
	}
};