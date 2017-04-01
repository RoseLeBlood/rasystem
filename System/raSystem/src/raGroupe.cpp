#include "..\include\raMain.h"

namespace System
{
	raGroup::raGroup(raSmartPointer<raDirectX> dx, IRAMesh *pVisual, int numInstancesToDraw,
			bool bAlpha, bool bAdditive, bool bDepthDisable, D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode)
			: raVisual(dx, bAlpha, bAdditive, bDepthDisable, fillMode, cullMode)
	{
		m_pVisual = pVisual;
		m_iNumInstances = MAX_INSTANCES;
		m_iNumInstancesToDraw = numInstancesToDraw;
		m_pInstanceData = NULL;

		InitInstanceMatrices();
	}

	raGroup::~raGroup(void)
	{
		SAFE_DELETE(m_pVisual);
	}
	void raGroup::InitInstanceMatrices()
	{
		#pragma omp parallel for
		for(int i = 0; i < MAX_INSTANCES; i++)
		{
			m_InstanceMatrices[i] = raMatrixIdentity();
		}
	}
	void raGroup::CreateRandomMatrices(float scaleX, float scaleY, float scaleZ, float offsetY)
	{
		#pragma omp parallel for
		for(int i = 0; i < MAX_INSTANCES; i++)
		{
			m_InstanceMatrices[i] = raGetRandMatrix(scaleX, scaleY, scaleZ, offsetY);
		}
	}
	bool raGroup::CreateVertexLayout()
	{
		const D3D11_INPUT_ELEMENT_DESC *desc = m_pVisual->GetVertexLayout();
		int numElements = m_pVisual->GetNumElements();
		int numElementsGroup = numElements + 4;

		D3D11_INPUT_ELEMENT_DESC* pLayoutGroupe = new D3D11_INPUT_ELEMENT_DESC[numElementsGroup];
		if(!pLayoutGroupe)
		{
			RERROR_OUTOFMEM();
			return false;
		}

		for(int i = 0; i < numElementsGroup; i++)
		{
			pLayoutGroupe[i] = desc[i];
		}
		UINT AlignedByteOffset = 0;
		for(int i = numElements; i < numElementsGroup; i++)
		{
			pLayoutGroupe[i].SemanticName = "mTransform";
			pLayoutGroupe[i].SemanticIndex = i - numElements;
			pLayoutGroupe[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			pLayoutGroupe[i].InputSlot = 1;
			pLayoutGroupe[i].AlignedByteOffset = AlignedByteOffset;
			pLayoutGroupe[i].InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			pLayoutGroupe[i].InstanceDataStepRate = 1;
			AlignedByteOffset += 16;
		}
		if(!GetMaterial(0)->CreateVertexLayout(pLayoutGroupe, numElementsGroup, &m_pVertexLayout))
		{
			delete[] pLayoutGroupe;
			RERROR("Beim Erstellen des VertexLayout");
			return false;
		}
		delete[] pLayoutGroupe;

		ROK("VertexLayout erstellt");
		return true;
	}
	bool raGroup::Create()
	{
		raVisual::Create();
		LoadInstanceData();
		return true;
	}
	void raGroup::LoadInstanceData()
	{
		D3D11_BUFFER_DESC bufferDesc =
		{
			m_iNumInstances * sizeof(raMatrix),
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_VERTEX_BUFFER,
			D3D11_CPU_ACCESS_WRITE,
			0
		};
		m_dx->GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_pInstanceData);

		D3D11_MAPPED_SUBRESOURCE MappedResource;
		m_dx->GetImmediateContext()->Map(m_pInstanceData, 0, D3D11_MAP_WRITE_DISCARD,
				NULL, &MappedResource);
		raMatrix* pMatrices = (raMatrix*)MappedResource.pData;

		memcpy(pMatrices, m_pInstanceList, m_iNumInstances * sizeof(raMatrix));

		m_dx->GetImmediateContext()->Unmap(m_pInstanceData, 0);
	}
	void raGroup::Destroy()
	{
		SAFE_RELEASE(m_pInstanceData);
		raVisual::Destroy();
	}
	bool raGroup::RenderMesh(LPCSTR techniqueName)
	{
		UINT Strides[2];
		UINT Offsets[2] = {0,0};
		Strides[0] = m_pVisual->GetStrideSize();
		Strides[1] = sizeof(raMatrix);

		ID3D11Buffer* pVB[2];
		pVB[0] = m_pVisual->GetVertexBuffer();
		pVB[1] = m_pInstanceData;

		m_dx->GetImmediateContext()->IASetVertexBuffers(0, 2, pVB, Strides, Offsets);
		m_dx->GetImmediateContext()->IASetIndexBuffer(m_pVisual->GetIndexBuffer(),
			m_pVisual->GetIndexBufferFormat(), 0);

		for(int subsets = 0; subsets < GetNumSubsets(); subsets++)
		{
			SDKMESH_SUBSET* pSubsets = GetSubset(subsets);

			D3D11_PRIMITIVE_TOPOLOGY PrimType =
				GetPrimitiveTopology(subsets);
			m_dx->GetImmediateContext()->IASetPrimitiveTopology(PrimType);

			GetMaterial(pSubsets->MaterialID)->Setup();

			ID3DX11EffectTechnique* pTech =
				GetMaterial(pSubsets->MaterialID)->GetEffectTechnique(techniqueName);

			D3DX11_TECHNIQUE_DESC techdesc;
			HRESULT hr = pTech->GetDesc(&techdesc);
			if(FAILED(hr))
			{
				RERROR_DX11("GetDesc", hr);
				return false;
			}

			for(UINT p = 0; p < techdesc.Passes; p++)
			{
				pTech->GetPassByIndex(p)->Apply(0, m_dx->GetImmediateContext());
				m_dx->GetImmediateContext()->DrawIndexedInstanced((UINT)pSubsets->IndexCount,
						m_iNumInstancesToDraw, (UINT)pSubsets->IndexStart,
						0, 0);
			}
		}
		return true;
	}
	UINT raGroup::AddInstance(const raMatrix& world)
	{
		if(m_iNumInstancesToDraw >= MAX_INSTANCES) return MAX_INSTANCES;

		m_InstanceMatrices[m_iNumInstancesToDraw] =
			m_InstanceMatrices[m_iNumInstancesToDraw] * world;

		UpdateInstanceMatrices();

		return m_iNumInstancesToDraw++;
	}
	void raGroup::SetWorldMatrix(const raMatrix& value)
	{
		for(int i = 0; i < MAX_INSTANCES; i++)
		{
			m_InstanceMatrices[i] = value;
		}
	}
	void raGroup::SetWorldMatrix(const raMatrix& value, int instanceNo)
	{
		m_InstanceMatrices[instanceNo] = value;
		UpdateInstanceMatrices();
	}
	const raMatrix* raGroup::GetWorldMatrix(int instanceNo)
	{
		return m_InstanceMatrices + instanceNo;
	}
	 void raGroup::UpdateInstanceMatrices()
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource;

		m_dx->GetImmediateContext()->Map(m_pInstanceData, 0, D3D11_MAP_WRITE_DISCARD,
			NULL, &MappedResource);

		D3DXMATRIX* pMatrices = (D3DXMATRIX*)MappedResource.pData;
		memcpy(pMatrices, m_pInstanceList, m_iNumInstances * sizeof(D3DXMATRIX));

		m_dx->GetImmediateContext()->Unmap(m_pInstanceData, 0);
	}
};