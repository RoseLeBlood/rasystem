//--------------------------------------------------------------------------------------
// File: Terrain.fx
//
// DX11 Tesselation
//
// 
//--------------------------------------------------------------------------------------
#include "MyEngine.fxh"
#include "Noise.fxh"

//float   g_fTessellationFactor = 10; //für Reference-Device
float   g_fTessellationFactor = 100;

struct VS_CONTROL_POINT_INPUT
{
    float3 Position		: POSITION;
};

struct VS_CONTROL_POINT_OUTPUT
{
    float3 Position		: POSITION;
};

VS_CONTROL_POINT_OUTPUT VSTerrain(VS_CONTROL_POINT_INPUT Input)
{
    VS_CONTROL_POINT_OUTPUT Output;
    Output.Position = Input.Position;
    return Output;
}

struct HS_CONSTANT_DATA_OUTPUT
{
    float Edges[4]			: SV_TessFactor;
    float Inside[2]			: SV_InsideTessFactor;
};

HS_CONSTANT_DATA_OUTPUT ConstantsHSTerrain(InputPatch<VS_CONTROL_POINT_OUTPUT, 1> ip,
    uint PatchID : SV_PrimitiveID )
{	
    HS_CONSTANT_DATA_OUTPUT Output = (HS_CONSTANT_DATA_OUTPUT) 0;
    Output.Edges[0] = Output.Edges[1] = Output.Edges[2] = Output.Edges[3] = g_fTessellationFactor;
    Output.Inside[0] = Output.Inside[1] = g_fTessellationFactor;
    return Output;
}

struct Terrain_CONTROL_POINT
{
    float3 Position	: POSITION;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")] //cw für Wireframe, sonst ccw!!
[outputcontrolpoints(1)]
[patchconstantfunc("ConstantsHSTerrain")]
Terrain_CONTROL_POINT HSTerrain(InputPatch<VS_CONTROL_POINT_OUTPUT,1> p, 
    uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID )
{
    Terrain_CONTROL_POINT Output = (Terrain_CONTROL_POINT)0;
    Output.Position = p[0].Position;
    return Output;
}

[domain("quad")]
VS_OUTPUT DSTerrain(HS_CONSTANT_DATA_OUTPUT input, 
    float2 UV : SV_DomainLocation, 
    const OutputPatch<Terrain_CONTROL_POINT, 1> patch)
{
    VS_OUTPUT Output = (VS_OUTPUT) 0;
    float3 WorldPos = float3(
        2 * UV.x - 1, 
        fbm(UV.x, UV.y), 
        2 * UV.y - 1); 
        
    WorldPos += patch[0].Position;        
    Output.Position = mul( float4(WorldPos,1), g_mWorldViewProjection);
    //Für Per-Pixel-Lightung wird Diffuse-Color hier nicht gebraucht
    /*
   	float4 n = FindFbmNormal(UV); 
    float3 vNormalWorldSpace = normalize(mul(n, (float3x3)g_mWorld)); 

    Output.Diffuse.rgb = max(0,dot(vNormalWorldSpace, g_vLightDir));  
    Output.Diffuse.a = 1.0f; 
    */
    Output.TextureUV = UV; 
    
    return Output;    
}

//Per Vertex Lighting: funktioniert bei niedrigem Tesselationsfaktor nur schlecht
float4 PSTerrain1(VS_OUTPUT In) : SV_Target 
{
    float s = fbm(In.TextureUV.x, In.TextureUV.y);
    float4 color = g_txColorsFromHeight.Sample(g_samWrap, 0.5f * s + g_bias);
    color.a = 1.0f;
    return color * In.Diffuse;    
}

//Per Pixel Lighting: auch bei sehr niedrigem Tesselationsfaktor gut
float4 PSTerrain(VS_OUTPUT In) : SV_Target 
{ 
    float s = fbm(In.TextureUV.x, In.TextureUV.y);
    float4 n = FindFbmNormal(In.TextureUV);
    
    float3 vNormalWorldSpace = normalize(mul(n, (float3x3)g_mWorld)); 
    
    float4 color = g_txColorsFromHeight.Sample(g_samWrap, 0.5f * (s + 1))  
        * max(0, dot(vNormalWorldSpace, g_vLightDir));
    color.a = 1.0f;

    return color;
}

technique11 Terrain
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_5_0, VSTerrain()));
        SetDomainShader(CompileShader(ds_5_0, DSTerrain()));
        SetHullShader(CompileShader(hs_5_0, HSTerrain()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PSTerrain()));
    }
};