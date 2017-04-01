#pragma once

namespace raSystem
{
	struct RAPI RAMESH_HEADER
	{
		UINT NumVertices;
		UINT NumTriangles;
		UINT NumMaterials;
	};

	class RAPI raMeshFile : public raEntity<VERTEXPOSITIONNORMALTEXTURED>
	{
	public:
		raMeshFile(raDirectX *dx, LPCSTR FileName) : raEntity(dx, NULL, false, false, 0)
		{
			m_ModelFileName = FileName;
		}
		//~raMeshFile(void);

		virtual void Destroy() {};
		virtual bool CreateMesh(void);
		virtual bool DestroyMesh() { return true; };
		virtual void SetupVertices() {};

	private:
		LPCSTR			m_ModelFileName;
		BYTE*			m_pStaticMeshData;
	};

	class RAPI raSDKMesh : public raVisual
	{ 
	public:
		raSDKMesh(raDirectX *dx, LPCSTR modelFileName,  
			LPCSTR effectFileName = "raEffects\\MyEngine.fx", 
			LPCSTR techniqueName = "RenderScene");
        
		raSDKMesh(raDirectX *dx, LPCSTR modelFileName,  raMaterial *pMaterial);


		virtual bool CreateMesh();  
		virtual bool DestroyMesh();
		virtual bool RenderMesh(LPCSTR techniqueName = ""); 


		virtual bool Intersects(
			CONST raVector3 *pRayPos,
			CONST raVector3 *pRayDir,
			float *pDist);

		const LPCSTR& GetModelFileName() const;  

		//IMesh Interface
		virtual ID3D11Buffer* GetVertexBuffer(){ return m_Mesh.GetVB11(0, 0);}
		virtual ID3D11Buffer* GetIndexBuffer() { return m_Mesh.GetIB11(0);}
		virtual UINT GetStrideSize() { return m_Mesh.GetVertexStride(0, 0);}
		virtual D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(UINT n){ 
			return m_Mesh.GetPrimitiveType11(
				(SDKMESH_PRIMITIVE_TYPE)m_Mesh.GetSubset(0, n)->PrimitiveType);
		}

		virtual DXGI_FORMAT GetIndexBufferFormat()
		{
			return DXGI_FORMAT_R32_UINT;
		}

		virtual UINT GetNumSubsets(){ return m_Mesh.GetNumSubsets(0);}
		virtual SDKMESH_SUBSET* GetSubset(UINT n){ return m_Mesh.GetSubset(0, n);}

		virtual UINT GetNumMaterials(){ return 1; }
		virtual raMaterial* GetMaterial(UINT n){ return m_pMaterials[0]; }

	protected:
		LPCSTR			m_ModelFileName;
		CDXUTSDKMesh	m_Mesh;

		bool			m_bExternalMaterial;
	};
};