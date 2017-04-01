#pragma once

	struct RAMESH_HEADER
	{
		UINT NumVertices;
		UINT NumTriangles;
		UINT NumMaterials;
	};
	// in Ordner raModel kommen die Models rein, raTexturen die Texturen des Models
	class RAPI raModel : public raEntity<VERTEXPOSITIONNORMALTEXTURED>
	{
	public:
		// IN directx Klasse, IN FileName => FileName des Models im Ordner raModel
		raModel(raSmartPointer<raDirectX> dx, LPCSTR FileName);
		//raModel(raDirectX* dx, void* MemData, int Size);
		//raModel(raDirectX* dx, raVFile *file);

		~raModel(void);

		virtual bool CreateMesh();
		virtual void SetupVertices() {};

	private:
		LPCSTR		m_Modelfilename;
		BYTE*		m_pMeshData;
	};