#include "..\include\raMain.h"

namespace System
{
	raModel::raModel(raSmartPointer<raDirectX> dx, LPCSTR FileName) : raEntity(dx.get(), NULL, false, false, 0)
	{
		m_Modelfilename = FileName;
	}
	raModel::~raModel()
	{
		m_pVertices = NULL;
		m_pIndices = NULL;
	}
	bool raModel::CreateMesh()
	{
		HANDLE hFile = CreateFile((std::string("raModel\\") +m_Modelfilename).c_str(), FILE_READ_DATA, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

		//Datei Größe
		LARGE_INTEGER FileSize;
		GetFileSizeEx(hFile, &FileSize);
		UINT nBytes = FileSize.LowPart;
		m_pMeshData = new BYTE[nBytes];

		// Daten lesen
		DWORD nBytesRead;
		ReadFile(hFile, m_pMeshData, nBytes, &nBytesRead, NULL);
		CloseHandle(hFile);

		assert(nBytesRead > 0);

		RAMESH_HEADER * pMeshHeader = (RAMESH_HEADER*)m_pMeshData;
		m_nIndices					= 3 * pMeshHeader->NumTriangles;
		m_nVertices					= pMeshHeader->NumVertices;
		m_numMaterials				= pMeshHeader->NumMaterials;

		m_pVertices					= (VERTEXPOSITIONNORMALTEXTURED*)(pMeshHeader + 1);
		m_pIndices					= (UINT16*)(((VERTEXPOSITIONNORMALTEXTURED*)m_pVertices) +
										m_nVertices);
		//...
		//Materialien
		m_numSubsets = m_numMaterials;
		m_pSubsets = new SDKMESH_SUBSET[m_numSubsets];
		m_pMaterials = new raMaterial* [m_numMaterials];

		raArray<UINT> *pSubsets;
		pSubsets = new raArray<UINT>[m_numSubsets];

		UINT* pNumChars = (UINT*) ( m_pIndices + m_nIndices);
		for(UINT matNo = 0; matNo < m_numMaterials; matNo++)
		{
			m_pSubsets[matNo].MaterialID = matNo;
			m_pSubsets[matNo].PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			if(*pNumChars > 0)
			{
				CHAR * pTextureFileName = (CHAR*)(pNumChars + 1);
				pTextureFileName[*pNumChars] = (CHAR)0;
				m_pMaterials[matNo] = new raTexturedMaterial(m_dx.get(), "raEffects\\MyEngine.fx", "RenderScene",
					(std::string("raTextur\\") + pTextureFileName).c_str());
				pNumChars = (UINT*)(pTextureFileName + *pNumChars);
			}
			else
			{
				m_pMaterials[matNo] = new raMaterial(m_dx.get(), "raEffects\\MyEngine.fx", "RenderWithMaterialColor");
				pNumChars += 1;
			}
		}
		// Subsets
		UINT16 *pFaceIndexes = (UINT16*)pNumChars;
		int idx = 0;
		for(UINT i = 0; i < pMeshHeader->NumTriangles; i++)
		{
			for(int j = 0; j < 3; i++)
			{
				pSubsets[*pFaceIndexes].Add(m_pIndices[idx++]);
			}
			pFaceIndexes++;
		}
		//Indices Umsotieren
		idx = 0;
		for(UINT matNo=0; matNo < m_numMaterials; matNo++)
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
		raEntity::CreateMesh();
		delete[] m_pMeshData;

		return true;
	}
}