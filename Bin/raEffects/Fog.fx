//--------------------------------------------------------------------------------------
// File: Fog.fx
//
// Nebel
//
//--------------------------------------------------------------------------------------
#include "MyEngine.fxh"

//--------------------------------------------------------------------------------------
// Globale Variablen
//--------------------------------------------------------------------------------------
cbuffer cbFog
{
    float    g_fogStart = 1.0f;
    float    g_fogEnd = 100.0f;
    float4   g_fogColor = float4(1, 1, 1, 1);
};

struct VS_OUTPUT_FOG
{
    float4 Position  : SV_Position;   //position
    float4 Diffuse   : COLOR0;        //color for gouraud and flat shading
    float2 TextureUV : TEXTURE0;      //texture coordinate
    float3 wPos      : TEXTURE1;      //world space pos
    float  fogDist   : FOGDISTANCE;   //distance used for fog calculations
};

//--------------------------------------------------------------------------------------
// Vertexshader
//--------------------------------------------------------------------------------------
VS_OUTPUT_FOG RenderFogVS(float4 vPos       : POSITION, 
                          float3 vNormal    : NORMAL,
                          float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT_FOG output = (VS_OUTPUT_FOG)0.0;

    //output our final position in clipspace
    float4 worldPos = mul(vPos, g_mWorld);
    float4 cameraPos = mul( worldPos, g_mView ); //Save cameraPos for fog calculations
    output.Position = mul( cameraPos, g_mProj );
    
    output.wPos = worldPos;
    output.fogDist = cameraPos.z;
    output.Diffuse = float4(1, 1, 1, 1); //keine Beleuchtung
    output.TextureUV = vTexCoord0;
    return output;
}

//
// Calculates fog factor based upon distance
//
float CalcFogFactor( float d )
{
    float fogCoeff = 1.0;
    fogCoeff = (g_fogEnd - d)/(g_fogEnd - g_fogStart);
    return clamp( fogCoeff, 0, 1 );
}

//--------------------------------------------------------------------------------------
// Pixelshader
//--------------------------------------------------------------------------------------
float4 RenderFogPS(VS_OUTPUT_FOG input) : SV_Target
{   
    //calculate the fog factor  
    float fog = CalcFogFactor( input.fogDist );

    //calculate the color based off of the normal, textures, etc
    float4 normalColor = g_MeshTexture.Sample(g_samWrap, input.TextureUV ) 
        * input.Diffuse;
    
    //since we don't have texldp, we must perform the w divide ourselves befor the texture lookup
    return fog * normalColor + (1.0 - fog)*g_fogColor;
}
//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 RenderFog
{
    pass p0
    {
        SetVertexShader( CompileShader(vs_4_0, RenderFogVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, RenderFogPS() ) );
        
        //SetDepthStencilState(EnableDepthWrite, 0 );
    }  
}
