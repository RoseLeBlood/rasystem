	struct VERTEXPOSITIONNORMALTEXTURED
	{
		raVector3 position;
		raVector3 normal;
		raVector2 texcoord;

		static D3D11_INPUT_ELEMENT_DESC* GetVertexLayout()
		{
			static D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
					0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			return layout;
		}

		static UINT GetNumElements(){ return 3; }
	};

	struct VERTEXPOSITIONNORMALTEXTUREDSKINNED
	{
		raVector3 position;
		BYTE weights[4];
		BYTE bones[4];
		raVector3 normal;
		raVector2 texcoord;

		static D3D11_INPUT_ELEMENT_DESC* GetVertexLayout()
		{
			static D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "WEIGHTS", 0, DXGI_FORMAT_R8G8B8A8_UNORM,
					0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "BONES", 0, DXGI_FORMAT_R8G8B8A8_UINT,
					0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
					0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			return layout;
		}

		static UINT GetNumElements(){ return 6; }
	};

	struct VERTEXPOSITIONCOLOR
	{
		raVector3 position;
		float color[3];

		static D3D11_INPUT_ELEMENT_DESC* GetVertexLayout()
		{
			static D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			return layout;
		}

		static UINT GetNumElements(){ return 2; }
	};

	struct VERTEXPOSITION
	{
		raVector3 position;

		static D3D11_INPUT_ELEMENT_DESC* GetVertexLayout()
		{
			static D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			return layout;
		}

		static UINT GetNumElements(){ return 1; }
	};

	struct VERTEXPOSITIONTEXTURED
	{
		raVector3 position;
		raVector2 texcoord;

		static D3D11_INPUT_ELEMENT_DESC* GetVertexLayout()
		{
			static D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
					0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			return layout;
		}

		static UINT GetNumElements(){ return 2; }
	};

	struct CUBEMAPVERTEX
	{
		raVector3 position;
		raVector3 texcoord;

		static D3D11_INPUT_ELEMENT_DESC* GetVertexLayout()
		{
			static D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			return layout;
		}

		static UINT GetNumElements(){ return 2; }
	};

	struct POINTSPRITEVERTEX
	{
		raVector3 position;
		float color[3];
		float radius;
		static D3D11_INPUT_ELEMENT_DESC* GetVertexLayout()
		{
			static D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "RADIUS", 0, DXGI_FORMAT_R32_FLOAT,
					0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			return layout;
		}

		static UINT GetNumElements(){ return 3; }
	};

	struct PARTICLEVERTEX
	{
		raVector3 position;
		raVector3 velocity;
		float color[3];
		float radius;

		static D3D11_INPUT_ELEMENT_DESC* GetVertexLayout()
		{
			static D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
					0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "RADIUS", 0, DXGI_FORMAT_R32_FLOAT,
					0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			return layout;
		}

		static UINT GetNumElements(){ return 4; }
	};
	struct VERTEXFONT
	{
		raVector2 vPos;
		raColor vColor;
		float tu, tv;

		static D3D11_INPUT_ELEMENT_DESC* GetVertexLayout()
		{
			static D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			return layout;
		}
		static UINT GetNumElements(){ return 3; }
	};