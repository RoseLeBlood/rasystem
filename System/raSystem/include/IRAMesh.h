#pragma once

	class RAPI IRAMesh
	{
	public:
		virtual bool CreateMesh() = 0;
		virtual bool DestroyMesh() = 0;
		virtual bool RenderMesh(LPCSTR techniqueName = "") = 0;

		virtual void CreateEffectVariables() = 0;
		virtual void DestroyEffectVariables() = 0;
		virtual void SetupEffectVariables(const raMatrix& pView, const raMatrix& pProj,
										  const raLight* pLight) = 0;

		virtual void CreateMaterials() = 0;
		virtual void DestroyMaterials() = 0;

		virtual UINT GetNumMaterials() = 0;
		virtual raMaterial* GetMaterial(UINT n) = 0;
		virtual UINT GetNumSubsets() = 0;
		virtual SDKMESH_SUBSET* GetSubset(UINT n) = 0;

		virtual const D3D11_INPUT_ELEMENT_DESC* GetVertexLayout() = 0;
		virtual UINT GetNumElements() = 0;

		virtual ID3D11Buffer* GetVertexBuffer() = 0;
		virtual ID3D11Buffer* GetIndexBuffer() = 0;
		virtual UINT GetStrideSize() = 0;
		virtual DXGI_FORMAT GetIndexBufferFormat() = 0;
		virtual D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(UINT n) = 0;

		virtual ~IRAMesh(){};
	};