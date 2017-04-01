#pragma once

	class RAPI raHoehenfeld : public raEntity<VERTEXPOSITIONNORMALTEXTURED>
	{
	public:
		raHoehenfeld(raSmartPointer<raDirectX> dx, int width, int depth, raMaterial* mat, float tileSize = 1.0f);
		~raHoehenfeld(void);

		virtual void SetupVertices();
		virtual void SetupIndices();

	protected:
		int m_PixelFormat;
	private:
		virtual float h(float x, float z) = 0; // w = Pixel Format ( 1024 x 1024 )
		virtual raVector3 normal(float x, float z) = 0; // Normale
	private:
		int m_width;
		int m_depth;

		float m_tileSize;
	};

	class RAPI raHeightMap : public raHoehenfeld
	{
	public:
		raHeightMap(raSmartPointer<raDirectX> dx, int width, int depth, raMaterial *mat, float tileSize = 1.0f,
			int PixelFormat = 1024,
			LPCSTR heightTexture = "raGrafik\\Heightmap.raw",
			LPCSTR bumpMapTexture = "raGrafik\\HeightNormals.raw");
		~raHeightMap();

	private:
		virtual float h(float x, float z);
		virtual raVector3 normal(float x, float z);

	private:
		BYTE* m_pHeightMap;
		BYTE* m_pBumpData;
		int	  m_PixelFormat;
	};