//--------------------------------------------------------------------------------------
// File: MyEngine.fxh
//
// Header für Effektdateien
//
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
shared cbuffer cbPerFrame
{
	//View und Proj für Glanzlichter (geht aber auch ohne (mit WorldView und WorldViewProjection))
	//außerdem für SkinnedInstancing, Proj auch für Cubemapping
	float4x4 g_mView                    : VIEW;
    float4x4 g_mProj                    : PROJECTION;

    float4x4 g_mInvView;                //für Partikeleffekt

	float3 g_vLightDir = float3(-1.5f, 0, 0);        //World Space
    float4 g_LightPos = float4( 1.5f, 0, 0, 1); 
	
    float4 g_LightPosViewSpace;                     //Position der Lichtquelle im Koordinatensystem der Kamera 
    float3 g_LightDirViewSpace;                     //Richtung der Lichtquelle im Koordinatensystem der Kamera 

	float4 g_LightDiffuse = float4(1, 1, 1, 1);  
    float4 g_LightAmbientColor = float4(0.01f, 0.01f, 0.01f, 1); 
    
    //Glanzlicht
    float4 g_LightSpecular = float4(1, 1, 1, 1);
    float g_SpecularPower = 8;
    
    //Punktlicht
    //float4 g_LightAttenuation = float4(1, 0, 0, 0); 
    float4 g_LightAttenuation = float4(0, 0, 0.04f, 0); 

    //Spotlicht    
    float g_cosHalfPhi   = 0.4f;  //äußerer Kegel
    float g_cosHalfTheta = 0.9f;  //innerer Kegel
    
    //Für Reflektion und Tessellation und Minimap    
    float3 g_cameraPos = float3(0, 0, 0);

    //Für Animation
	float    g_fTime;                   
}

struct Light
{
    float4 Position; 
	float3 Direction;                  
	
    float4 PosViewSpace;         
    float3 DirViewSpace;         

    //Punktlicht
    float4 Attenuation; 

    //Spotlicht    
    float cosHalfPhi;  //äußerer Kegel
    float cosHalfTheta;  //innerer Kegel
    
	float4 Diffuse;              
    float4 Ambient; 
    float4 Specular;
};

shared cbuffer cbLights
{
    Light g_Lights[4]; 
};

#define MAX_BONE_MATRICES 255
shared cbuffer cbPerObject
{
	float4x4 g_mWorld 					: WORLD;     
	float4x4 g_mWorldView               : WORLDVIEW;       		
	float4x4 g_mWorldViewProjection 	: WORLDVIEWPROJECTION;  
    //float4x4 g_mWorldInverseTranspose : WORLDINVERSETRANSPOSE; 
    
    //Für Lightmapping    
    float4x4 g_mLightViewProjection;

    //Für Animation
    matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
    int g_NumKeys = 2000; 
    int g_StartKey = 0; 
}

shared cbuffer cbPerMaterial
{
	float4 g_MaterialAmbientColor = float4(0.5f, 0.5f, 0.5f, 1); 
	float4 g_MaterialDiffuseColor = float4(1, 1, 1, 1);      
	
    bool g_bSelected = false;
}

shared cbuffer cbQuads
{
    //Für Full-Screen-Rendering    
    float4 QuadPositionTexCoordsFullScreen[] = 
    {
	    float4(-1,-1,0,1),
	    float4(-1,+1,0,0),
	    float4(+1,-1,1,1),
	    float4(+1,+1,1,0),
    };

    float4 QuadPositionTexCoords[] = 
    {
		//Linke obere Ecke
	    float4(-1,   0.5f,0,1),
	    float4(-1,   1,   0,0),
	    float4(-0.5f,0.5f,1,1),
	    float4(-0.5f,1,   1,0),
    };
}

shared cbuffer cbCubeMap
{
	matrix g_mViewCM[6]; // View matrices for cube map rendering
};

shared Texture2D   g_MeshTexture;              //nicht gebraucht für Cubemap        
shared Texture2D   g_PlayerIconTexture;                
shared Texture2D   g_shadowTexture;            //nur für Lightmapping
shared Texture2D   g_HeightTexture;   
shared Texture2D   g_NormalMap;   

shared Texture2D g_txAnimations    : ANIMATIONS;

#define MAX_INSTANCES 500                      //für Skinned Instnacing
shared cbuffer cbInstanceData
{
	uint g_StartKeyArray[MAX_INSTANCES];
	uint g_NumKeysArray [MAX_INSTANCES];
}

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
shared SamplerState g_samWrap
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

shared SamplerState g_samClamp
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};
//--------------------------------------------------------------------------------------
// Helper
//--------------------------------------------------------------------------------------

float3 FindNormal( float3 Value ) //Nur im Pixelshader!!
{
    float3 dWorldX = ddx(Value);
    float3 dWorldY = ddy(Value);
    float3 norm = normalize( cross( dWorldX, dWorldY ) );
    return norm;
}

//--------------------------------------------------------------------------------------
// Struktur für Rendern mit Textur
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : SV_Position;   // vertex position 
    float4 Diffuse    : COLOR0;        // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV  : TEXCOORD0;     // vertex texture coords 
};

//--------------------------------------------------------------------------------------
// Struktur für Beleuchtung im Pixelshader
//--------------------------------------------------------------------------------------
struct VS_OUTPUT_PERPIXELLIGHTING
{
    float4 Position   : SV_Position;   // vertex position 
    float3 WorldPos   : POSITIONT;  
    float2 TextureUV  : TEXCOORD0;     // vertex texture coords 
};

//--------------------------------------------------------------------------------------
// Struktur für Rendern in Lightmap und DepthMap
//--------------------------------------------------------------------------------------
struct VS_OUTPUT_ToLightMap
{
    float4 Position		        : SV_Position;   
    float2 vertexPosition		: POSITION;    
};

//--------------------------------------------------------------------------------------
// Struktur für Rendern in Cubemap und Deferred Lighting
//--------------------------------------------------------------------------------------
struct PS_CUBEMAP_IN
{
    float4 Position   : SV_POSITION;     
    float4 Diffuse    : COLOR0;
    float2 TextureUV  : TEXCOORD0;       
    uint RTIndex      : SV_RenderTargetArrayIndex;
};

//--------------------------------------------------------------------------------------
// Struktur für Spotlicht
//--------------------------------------------------------------------------------------
struct VS_OUTPUT_SPOT
{
    float4 Position           : SV_Position;   // vertex position 
    float2 TextureUV          : TEXCOORD0;     // vertex texture coords 
    float4 PositionViewSpace  : POSITIONT;     
    float3 NormalViewSpace    : NORMALT;  
};

//--------------------------------------------------------------------------------------
// Vertexshader mit Textur
//--------------------------------------------------------------------------------------

shared VS_OUTPUT RenderWithTextureVS(float4 vPos       : POSITION, 
                                     float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    Output.Position = mul(vPos, g_mWorldViewProjection);
    Output.Diffuse = g_MaterialDiffuseColor * g_LightDiffuse + 
	    g_MaterialAmbientColor;   
    Output.Diffuse.a = 1.0f; 
    Output.TextureUV = vTexCoord0; 
    return Output;
}

//--------------------------------------------------------------------------------------
// Vertexshader mit Textur, Full Screen
//--------------------------------------------------------------------------------------
shared VS_OUTPUT RenderWithTextureFullScreenVS(uint VertexId: SV_VertexID)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
	Output.Position = float4(QuadPositionTexCoordsFullScreen[VertexId].xy, 0, 1);
    
    Output.Diffuse = g_MaterialDiffuseColor * g_LightDiffuse + 
        g_MaterialAmbientColor;   
    Output.Diffuse.a = 1.0f; 
    
	Output.TextureUV = QuadPositionTexCoordsFullScreen[VertexId].zw;
    
    return Output;
}

shared float4 SimplePS( VS_OUTPUT In ) : SV_Target
{ 
    return In.Diffuse;
}

//--------------------------------------------------------------------------------------
// Pixelshader mit Vertexfarbe
//--------------------------------------------------------------------------------------
//shared float4 RenderWithColorPS(VS_OUTPUT In) : SV_Target
//{ 
//    return In.Diffuse;
//}

//--------------------------------------------------------------------------------------
// Pixelshader ohne Beleuchtung
//--------------------------------------------------------------------------------------
shared float4 RenderSceneFullBrightPS(VS_OUTPUT In) : SV_Target
{ 
    return g_MeshTexture.Sample(g_samWrap, In.TextureUV);
}

//--------------------------------------------------------------------------------------
// Pixelshader mit Beleuchtung
//--------------------------------------------------------------------------------------
shared float4 RenderScenePS(VS_OUTPUT In) : SV_Target
{ 
    return g_MeshTexture.Sample(g_samWrap, In.TextureUV) * In.Diffuse;
}

//-----------------------------------------------------------------------------
// Lightmapping
//-----------------------------------------------------------------------------
VS_OUTPUT_ToLightMap RenderToLightMapVS(float4 inPosition  : POSITION)
{
    VS_OUTPUT_ToLightMap Output = (VS_OUTPUT_ToLightMap)0;
    Output.Position = mul(inPosition, g_mWorldViewProjection); //übergeben wird hier die Matrix aus Sicht der Lichtquelle!
    //Nur x wird gebraucht - Skalierung in y speichern
    Output.vertexPosition.x = Output.Position.z; 
    Output.vertexPosition.y = Output.Position.w; 
    return Output;    
}

float4 RenderToLightMapPS(VS_OUTPUT_ToLightMap IN) : SV_Target
{
    return IN.vertexPosition.x / IN.vertexPosition.y; //0 bis 1
}
