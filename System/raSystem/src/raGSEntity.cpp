#include "..\include\raMain.h"

namespace System
{
	raGSEntity::raGSEntity(raSmartPointer<raDirectX> dx, raSmartPointer<raMaterial> pMaterial) : raEntity(dx, NULL, false, false, 0)
	{
		m_numMaterials = 2;
		m_pMaterials = new raMaterial*[m_numMaterials];
		if(pMaterial.get())
			m_pMaterials[0] = pMaterial.get();
		else
			m_pMaterials[0] = new raProceduralMaterial(dx, 16, "FbmLighted");

		m_pMaterials[1] = new raMaterial(dx,
			"raEffects\\TesselationEffect.fx", "CreateGeometry");

		m_nVertices = 4;
		m_nIndices = 6;

		m_numSubsets = 1;
		m_pSubsets = new SDKMESH_SUBSET[m_numSubsets];
		m_pSubsets[0].IndexStart = 0;
		m_pSubsets[0].IndexCount = m_nIndices;
		m_pSubsets[0].VertexStart = 0;
		m_pSubsets[0].VertexCount = m_nVertices;
		m_pSubsets[0].MaterialID = 0;

		m_pStreamTo = NULL;
		m_pDrawFrom = NULL;

		m_SplitRatio = 5;
		m_bDepthDisable = true;
	}

	void raGSEntity::SetupVertices()
	{
	 m_pVertices =  new VERTEXPOSITIONTEXTURED [m_nVertices];

		m_pVertices[0].position.x = -1.0f;
		m_pVertices[0].position.y = 0.0f;
		m_pVertices[0].position.z = -1.0f;

		m_pVertices[1].position.x = -1.0f;
		m_pVertices[1].position.y = 0.0f;
		m_pVertices[1].position.z = 1.0f;

		m_pVertices[2].position.x = 1.0f;
		m_pVertices[2].position.y = 0.0f;
		m_pVertices[2].position.z = -1.0f;

		m_pVertices[3].position.x = 1.0f;
		m_pVertices[3].position.y = 0.0f;
		m_pVertices[3].position.z = 1.0f;

		for(int i = 0; i < 4; i++)
		{
			m_pVertices[i].texcoord.x =  0.5f * ((float)m_pVertices[i].position.x + 1);
			m_pVertices[i].texcoord.y =  0.5f * ((float)m_pVertices[i].position.z + 1);
		}
	}
	void raGSEntity::SetupIndices()
	{
		m_pIndices = new UINT16[m_nIndices];

		m_pIndices[0] = 0;
		m_pIndices[1] = 1;
		m_pIndices[2] = 2;
		m_pIndices[3] = 3;
		m_pIndices[4] = 2;
		m_pIndices[5] = 1;
	}

	bool raGSEntity::Create()
	{
		raEntity::Create();

		m_pCreateGeometryTechnique = m_pMaterials[1]->GetEffectTechnique();
		m_SplitRatioVariable = m_pMaterials[1]->GetEffect()->
			GetVariableByName("g_split_ratio" )->AsScalar();

		const UINT MaxVertices = 300000;
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = MaxVertices * GetStrideSize();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		m_dx->GetDevice()->CreateBuffer( &bd, NULL, &m_pStreamTo) ;
		m_dx->GetDevice()->CreateBuffer( &bd, NULL, &m_pDrawFrom) ;

		return true;
	}
	void raGSEntity::Destroy()
	{
		raEntity::Destroy();
		SAFE_RELEASE( m_pStreamTo );
		SAFE_RELEASE( m_pDrawFrom );
	}
	bool raGSEntity::RenderMesh(LPCSTR techniqueName)
	{
		ID3DX11EffectTechnique* pTechnique =
				m_pMaterials[0]->GetEffectTechnique(techniqueName);

		m_bOriginalBufferVerwenden = true;
		//Mehrfach aufrufen, um mehrere Levels zu erzeugen
		for(int l = 16; l > 0; l--)
		{
			CreateGeometry();
		}

		m_pMaterials[0]->Setup();

		m_dx->GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		UINT Strides[1];
		UINT Offsets[1];
		Strides[0] = GetStrideSize();
		Offsets[0] = 0;

		m_dx->GetImmediateContext()->IASetVertexBuffers( 0, 1, &m_pDrawFrom, Strides, Offsets);

		D3DX11_TECHNIQUE_DESC techDesc;
		pTechnique->GetDesc( &techDesc );

		for(UINT p = 0; p < techDesc.Passes; ++p )
		{
			pTechnique->GetPassByIndex(p)->Apply(0, m_dx->GetImmediateContext());
			m_dx->GetImmediateContext()->DrawAuto();
		}
		return true;
	}
	void raGSEntity::CreateGeometry()
	{
		ID3D11Buffer *pBuffers[1];

		if(m_bOriginalBufferVerwenden)
		{
			pBuffers[0] = m_pVB;
			m_dx->GetImmediateContext()->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R16_UINT, 0);
		}
		else pBuffers[0] = m_pDrawFrom;

		m_dx->GetImmediateContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		UINT Strides[1];
		UINT Offsets[1];
		Strides[0] = GetStrideSize();
		Offsets[0] = 0;
		m_dx->GetImmediateContext()->IASetVertexBuffers(0, 1, pBuffers, Strides, Offsets);

		//Stream Output Traget
		pBuffers[0] = m_pStreamTo;
		m_dx->GetImmediateContext()->SOSetTargets(1, pBuffers, Offsets);

		D3DX11_TECHNIQUE_DESC techDesc;
		m_pCreateGeometryTechnique->GetDesc(&techDesc);

		ID3D11DepthStencilState* pOldDepthStencilState = NULL;
		UINT StencilRef = 0;
		m_dx->GetImmediateContext()->OMGetDepthStencilState(&pOldDepthStencilState, &StencilRef);
		m_dx->GetImmediateContext()->OMSetDepthStencilState(m_pDepthStencilState, 0);

		if(m_SplitRatioVariable) m_SplitRatioVariable->SetFloat(m_SplitRatio);

		for(UINT p = 0; p < techDesc.Passes; ++p )
		{
			m_pCreateGeometryTechnique->GetPassByIndex(p)->Apply(0, m_dx->GetImmediateContext());

			if(m_bOriginalBufferVerwenden)
			{
				m_dx->GetImmediateContext()->DrawIndexed(m_nIndices, 0, 0);
				m_bOriginalBufferVerwenden = false;
			}
			else m_dx->GetImmediateContext()->DrawAuto();
		}

		m_dx->GetImmediateContext()->OMSetDepthStencilState(pOldDepthStencilState, StencilRef);
		SAFE_RELEASE(pOldDepthStencilState);

		//Stream Output Traget zurücksetzen
		pBuffers[0] = NULL;
		m_dx->GetImmediateContext()->SOSetTargets(1, pBuffers, Offsets);

		//Buffer tauschen
		ID3D11Buffer* pTemp = m_pDrawFrom;
		m_pDrawFrom = m_pStreamTo;
		m_pStreamTo = pTemp;
	}
};