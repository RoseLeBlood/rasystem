
#include "..\include\raMain.h"

namespace raSystem
{
	bool raMeshFile::CreateMesh()
	{
		HANDLE hFile = CreateFile(m_ModelFileName, 
		FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, 
		FILE_FLAG_SEQUENTIAL_SCAN, NULL);

		if(!hFile)
			RERROR_FILE(m_ModelFileName, false);

		//Dateigröße ermitteln
		LARGE_INTEGER FileSize;
		GetFileSizeEx(hFile, &FileSize);
		UINT nBytes = FileSize.LowPart;
		m_pStaticMeshData = new BYTE[nBytes];

		//Daten lesen
		DWORD nBytesRead;
		ReadFile(hFile, m_pStaticMeshData, nBytes, &nBytesRead, NULL);
		CloseHandle(hFile); 

		assert(nBytesRead > 0); 

		RAMESH_HEADER* pMeshHeader = (RAMESH_HEADER*)m_pStaticMeshData;
		if(!pMeshHeader)
			RERROR_OUTOFMEM(false);

			m_nIndices                 = 3 * pMeshHeader->NumTriangles;
			m_nVertices                = pMeshHeader->NumVertices;
			m_numMaterials             = pMeshHeader->NumMaterials; 

		m_pVertices = (VERTEXPOSITIONNORMALTEXTURED*)(pMeshHeader + 1);
		m_pIndices  = (UINT16*)(((VERTEXPOSITIONNORMALTEXTURED*)m_pVertices) +  
			m_nVertices);

		m_numSubsets = m_numMaterials;
		m_pSubsets = new SDKMESH_SUBSET[m_numSubsets];
		m_pMaterials = new raMaterial* [m_numMaterials];

		raArray<UINT> *pSubsets;
			pSubsets = new raArray<UINT>[m_numSubsets];
			if(!pSubsets)
				RERROR_OUTOFMEM(false);
        
			UINT* pNumChars = (UINT*) (m_pIndices + m_nIndices); 
			for(UINT matNo = 0; matNo < m_numMaterials; matNo++)
			{
				m_pSubsets[matNo].MaterialID = matNo;
				m_pSubsets[matNo].PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				if(*pNumChars > 0)
				{
					CHAR* pTextureFilename = (CHAR*) (pNumChars + 1); 
					char DiffuseTexture[MAX_TEXTURE_NAME];
					DiffuseTexture[*pNumChars] = (char) 0;
					m_pMaterials[matNo] = new raTexturedMaterial(m_dx, 
						"raEffects\\MyEngine.fx", "RenderScene", DiffuseTexture); 

						pNumChars = (UINT*) (pTextureFilename + *pNumChars);
				}
				else     
				{
					m_pMaterials[matNo] = new raMaterial(m_dx, 
						"raEffects\\MyEngine.fx", 
						"RenderWithMaterialColor");
					pNumChars += 1; 
				} 
			}

			UINT16* pFaceIndexes = (UINT16*)pNumChars;
			int idx = 0; 
			for(UINT i = 0; i < pMeshHeader->NumTriangles; i++)
			{
				for(int j = 0; j < 3; j++)
				{
					pSubsets[*pFaceIndexes].Add(m_pIndices[idx++]);
				}
				pFaceIndexes++;
			}

			//Indices umsortieren
			idx = 0; 
			for(UINT matNo = 0; matNo < m_numMaterials; matNo++)
			{
				m_pSubsets[matNo].MaterialID = matNo;
				m_pSubsets[matNo].IndexStart = idx;   

				m_pSubsets[matNo].IndexCount = pSubsets[matNo].GetSize();
				for(int i = 0; i < pSubsets[matNo].GetSize(); i++)
				{
				   m_pIndices[idx++] = pSubsets[matNo][i]; 
				}
			}
			delete[] pSubsets;

			if(!raEntity::CreateMesh())
				RERROR("raEntity Create - MeshFile()", false);
			delete[] m_pStaticMeshData; 

			CreateVertexBuffer();
			CreateIndexBuffer();
			ComputeBoundingBox();

			ROK("ram File galden");
		return true;
	}
	//-------------------------------------------------------------------------------------
	raSDKMesh::raSDKMesh(raDirectX* dx,   LPCSTR modelFileName,  
		LPCSTR effectFileName, LPCSTR techniqueName) : raVisual(dx)
	{
		m_ModelFileName = modelFileName;
		m_bExternalMaterial = false; 
		m_numMaterials = 1; 
		m_pMaterials = new raMaterial*[1];
		m_pMaterials[0] = 
			new raMaterial(dx, effectFileName, techniqueName);
	}

	raSDKMesh::raSDKMesh(raDirectX* dx, LPCSTR modelFileName, raMaterial *pMaterial): raVisual(dx)
	{
		m_ModelFileName = modelFileName;
		m_bExternalMaterial = true;
		m_numMaterials = 1; //auch wenn für pMaterial NULL übergeben wird, Speicherplatz anlegen
		m_pMaterials = new raMaterial*[1];
		m_pMaterials[0] = pMaterial;
	}

	bool raSDKMesh::CreateMesh()
	{
		HRESULT hr = m_Mesh.Create(m_dx, m_ModelFileName);
		if(FAILED(hr))
			RERROR_DX11("CreateMesh", hr, false);
		return true;
	} 

	bool raSDKMesh::DestroyMesh()
	{
		m_Mesh.Destroy();
		return true;
	}  

	bool raSDKMesh::RenderMesh(LPCSTR techniqueName)
	{
		raMaterial* pMaterial = m_pMaterials[0];
		ID3DX11EffectTechnique* pTechnique = 
			pMaterial->GetEffectTechnique(techniqueName);
		pMaterial->Setup();  

		pTechnique->GetPassByIndex(0)->Apply(0, m_dx->GetImmediateContext());
	
		if(m_bExternalMaterial)
		{
			m_Mesh.Render(-1, -1, -1);
		}
		else
		{
			m_Mesh.Render(0, -1, -1);
		}
		return true;
	} 

	bool raSDKMesh::Intersects(CONST raVector3 *pRayPos, 
		CONST raVector3 *pRayDir, float *pDist)
	{
		UINT64 nIndices = m_Mesh.GetNumIndices(0);

		VERTEXPOSITIONNORMALTEXTURED* pVertices = (VERTEXPOSITIONNORMALTEXTURED*)
			m_Mesh.GetRawVerticesAt(0);
    
		BYTE* pIndices = m_Mesh.GetRawIndicesAt(0);

		bool bIntersects = false; 
		FLOAT mindist = FLT_MAX;
		FLOAT u = 0, v = 0, dist = false; //u, v nicht verwendet

		for (UINT64 idx = 0; idx < nIndices; idx += 3)
		{
			raVector3 p[3]; 
			if(m_Mesh.GetIBFormat11(0) == DXGI_FORMAT_R16_UINT)
			{
				UINT16* pIndices16 = (UINT16*)pIndices;
				for(int i = 0; i < 3; i++)
					p[i] = (pVertices + pIndices16[idx + i])->position;
			}
			else
			{
				UINT32* pIndices32 = (UINT32*)pIndices;
				for(int i = 0; i < 3; i++)
					p[i] = (pVertices + pIndices32[idx + i])->position;
			}

			if(D3DXIntersectTri((D3DXVECTOR3 *)&p[0], (D3DXVECTOR3 *)&p[1], (D3DXVECTOR3 *)&p[2], 
			   (D3DXVECTOR3 *) pRayPos,(D3DXVECTOR3 *) pRayDir, &u, &v, &dist))
			{
				bIntersects = true; 
				if(dist < mindist) 
				{
					mindist = dist;
				}
			}
		}
		*pDist = mindist; 
		return bIntersects; 
	}
};
