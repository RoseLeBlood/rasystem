#include "..\include\raMain.h"

namespace System
{
	raEntitySDKMaterial::raEntitySDKMaterial(raSmartPointer<raDirectX> dx, LPCSTR effectFileName, LPCSTR Technique,
			SDKMESH_MATERIAL* mat) : raTexturedMaterial(dx.get(), effectFileName, Technique)
	{
		m_pSDKMaterial = mat;
	}
	bool raEntitySDKMaterial::Create()
	{
		raMaterial::Create();
		m_pRV = m_pSDKMaterial->pDiffuseRV11;
		if(m_pRV) m_pRV->AddRef();

		m_ptxDiffuseVariable = m_pEffect->GetVariableByName(m_effectVariableName)->AsShaderResource();

		return true;
	}
	void raEntitySDKMaterial::Setup()
	{
		if(m_pMaterialDiffuseColorVariable)
			m_pMaterialDiffuseColorVariable->SetFloatVector(m_pSDKMaterial->Diffuse);

		if(m_pMaterialAmbientVariable)
			m_pMaterialAmbientVariable->SetFloatVector(m_AmbientColor);

		if(this->m_ptxDiffuseVariable && !IsErrorResource(m_pRV))
			m_ptxDiffuseVariable->SetResource(m_pRV);
	}
	//-------------------------------------------------------------------
	raEntitySDKMesh::raEntitySDKMesh(raSmartPointer<raDirectX> dx,
            LPCSTR modelFileName) :
        raVisual(dx)
    {
		m_ModelFileName = modelFileName;
    }

    bool raEntitySDKMesh::CreateMesh()
    {
        m_Mesh.Create(m_dx.get(), m_ModelFileName);

        m_numMaterials = m_Mesh.GetNumMaterials();
        m_pMaterials = new raMaterial*[m_numMaterials];
        for(DWORD i = 0; i < m_numMaterials; i++)
        {
            m_pMaterials[i] = new raEntitySDKMaterial(m_dx,
			    EffectfileName.c_str(),
				TexturedTechniqueName.c_str(),
                m_Mesh.GetMaterial(i));
        }
		return true;
    }

    bool raEntitySDKMesh::DestroyMesh()
    {
	    m_Mesh.Destroy();
		return true;
    }

    bool raEntitySDKMesh::RenderMesh(LPCSTR techniqueName)
    {
        SDKMESH_MESH* pMesh  = m_Mesh.GetMesh(0);

        if( pMesh->NumVertexBuffers > 1)
            return false;

        UINT Strides[1];
        UINT Offsets[1];
        ID3D11Buffer* pVB[1];

        pVB[0] =  m_Mesh.GetVB11(0, 0);
        Strides[0] = m_Mesh.GetVertexStride(0, 0);
        Offsets[0] = 0;

        m_dx->GetImmediateContext()->IASetVertexBuffers(0, 1, pVB, Strides, Offsets );
        m_dx->GetImmediateContext()->IASetIndexBuffer(GetIndexBuffer(), GetIndexBufferFormat(), 0 );

        for( UINT subset = 0; subset < GetNumSubsets(); subset++) //Nicht m_numSubsets - das ist hier 0!
        {
            SDKMESH_SUBSET* pSubset = m_Mesh.GetSubset(0, subset);
            UINT materialNo = pSubset->MaterialID;

            raMaterial* pMaterial = m_pMaterials[materialNo];
            ID3DX11EffectTechnique* pTechnique =
                pMaterial->GetEffectTechnique(techniqueName);

			if(pTechnique->IsValid())
				//Beim Rendern in Cubemap: Nichts tun, falls Material ReflectiveMaterial ist und
				//vergessen wurde, das Objekt auszublenden - oder
				//vergessen wurde, dem Material eine CubemapTechnique zu geben
			{
				D3DX11_TECHNIQUE_DESC techDesc;
				pTechnique->GetDesc( &techDesc );

				pMaterial->Setup();

				D3D11_PRIMITIVE_TOPOLOGY PrimType = m_Mesh.GetPrimitiveType11(
					(SDKMESH_PRIMITIVE_TYPE)pSubset->PrimitiveType);
				m_dx->GetImmediateContext()->IASetPrimitiveTopology(PrimType);

				UINT indexStart = (UINT)pSubset->IndexStart;
				UINT indexCount = (UINT)pSubset->IndexCount;

				for( UINT p = 0; p < techDesc.Passes; ++p )
				{
					pTechnique->GetPassByIndex(p)->Apply(0, m_dx->GetImmediateContext());
					m_dx->GetImmediateContext()->DrawIndexed(indexCount, indexStart, 0);
				}
			}
        }
		return true;
    }

    bool raEntitySDKMesh::Intersects(const raVector3 *pRayPos,
        const raVector3 *pRayDir, float *pDist)
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
            D3DXVECTOR3 p[3];
            if(GetIndexBufferFormat() == DXGI_FORMAT_R16_UINT)
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

            if(D3DXIntersectTri(&p[0], &p[1], &p[2],
                (D3DXVECTOR3*)pRayPos, (D3DXVECTOR3*)pRayDir, &u, &v, &dist))
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

    LPCSTR raEntitySDKMesh::GetModelFileName()
    {
        return m_ModelFileName;
    }
};