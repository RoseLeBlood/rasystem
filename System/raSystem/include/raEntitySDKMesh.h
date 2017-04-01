#pragma once

	class RAPI raEntitySDKMaterial : public raTexturedMaterial
	{
	public:
		raEntitySDKMaterial(raSmartPointer<raDirectX> dx, LPCSTR effectFileName, LPCSTR Technique,
			SDKMESH_MATERIAL* mat);

		virtual bool Create();
		virtual void Setup();

	protected:
		SDKMESH_MATERIAL* m_pSDKMaterial;
	};
	class RAPI raEntitySDKMesh : public raVisual
	{
	public:
		raEntitySDKMesh(raSmartPointer<raDirectX> dx,
            LPCSTR modelFileName);

        virtual bool Intersects(
            const raVector3 *pRayPos,
            const raVector3 *pRayDir,
            float *pDist);

        LPCSTR GetModelFileName();

        //IRAMesh Interface
        virtual bool CreateMesh();
        virtual bool DestroyMesh();
        virtual bool RenderMesh(LPCSTR techniqueName = "");

        virtual ID3D11Buffer* GetVertexBuffer(){ return m_Mesh.GetVB11(0, 0);}
        virtual ID3D11Buffer* GetIndexBuffer() { return m_Mesh.GetIB11(0);}
        virtual UINT GetStrideSize() { return m_Mesh.GetVertexStride(0, 0);}
        virtual D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(UINT n)
		{
            return m_Mesh.GetPrimitiveType11(
                (SDKMESH_PRIMITIVE_TYPE)m_Mesh.GetSubset(0, n)->PrimitiveType);
        }
        virtual DXGI_FORMAT GetIndexBufferFormat()
        {
            return DXGI_FORMAT_R32_UINT;
        }
        virtual UINT GetNumSubsets(){ return m_Mesh.GetNumSubsets(0);}
        virtual SDKMESH_SUBSET* GetSubset(UINT n){ return m_Mesh.GetSubset(0, n);}
        virtual UINT GetNumMaterials(){ return m_numMaterials;}
        virtual raMaterial* GetMaterial(UINT n){ return m_pMaterials[n];}

	protected:
		LPCSTR m_ModelFileName;
        CDXUTSDKMesh m_Mesh;
	};