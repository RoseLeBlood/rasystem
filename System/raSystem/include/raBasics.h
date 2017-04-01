#pragma once

	struct INDEX
	{
	};
	class RAPI raTriangle: public raEntity<VERTEXPOSITIONCOLOR, false>
	{
		public:
			raTriangle(raSmartPointer<raDirectX> dx);
			virtual void SetupVertices();
	};
	class RAPI raQuad: public raEntity<VERTEXPOSITIONCOLOR>
	{
		public:
			raQuad(raSmartPointer<raDirectX> dx);
			raQuad(raSmartPointer<raDirectX> dx,
				raMaterial *pMaterial);
			virtual void SetupVertices();
			virtual void SetupIndices();
	};

	class RAPI raCube: public raEntity<VERTEXPOSITIONCOLOR>
	{
		public:
			raCube(raSmartPointer<raDirectX> dx);

			virtual void SetupVertices();
			virtual void SetupIndices();
	};

	class RAPI raSphere: public raEntity<VERTEXPOSITIONNORMALTEXTURED>
	{
		private:
			float m_r;
			int m_width, m_height;
		public:
			raSphere(raSmartPointer<raDirectX>,
				int width, int height, float radius = 1.0f,
				raMaterial* pMaterial = NULL);

		virtual void SetupVertices();
		virtual void SetupIndices();
	};

	class RAPI raRotationskoerper: public raEntity<VERTEXPOSITIONCOLOR>
	{
		private:
			int m_height;
			int m_nSlices;
			float r(int y){return 1.0f;}

		public:
		   raRotationskoerper(raSmartPointer<raDirectX>,   int height);
			virtual void SetupVertices();
			virtual void SetupIndices();
	};

	class RAPI raLineList: public raEntity<VERTEXPOSITIONCOLOR>
	{
		private:
			 int m_level;
			void makeChildren(VERTEXPOSITIONCOLOR* vertices, D3DXVECTOR3 v0,
					  D3DXVECTOR3 v1, int& n, int level);
		public:
			raLineList(raSmartPointer<raDirectX>,   int level);
			virtual void SetupVertices();
			virtual void RenderMesh(ID3D11DeviceContext* pd3dImmediateContext,ID3DX11EffectTechnique* pTechnique);
	};

	class RAPI raTexturedQuad: public raEntity<VERTEXPOSITIONTEXTURED>
	{
		public:
			 raTexturedQuad(raSmartPointer<raDirectX> dx,
				raMaterial* pMaterial, bool bAlpha = false);
			virtual void SetupVertices();
			virtual void SetupIndices();
	};

	class RAPI raAchsenkreuz: public raEntity<VERTEXPOSITIONCOLOR>
	{
		public:
			raAchsenkreuz(raSmartPointer<raDirectX> dx);

			virtual void SetupVertices();
	};