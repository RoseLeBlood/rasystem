//--------------------------------------------------------------------------------------
// File: SkyBoxEffect.fx
//
// Rendern einer Skybox mit Cubemaptextur
//
// 
//--------------------------------------------------------------------------------------

#include "MyEngine.fxh"

//--------------------------------------------------------------------------------------
// Globale Variablen und Strukturen
//--------------------------------------------------------------------------------------
TextureCube g_SkyboxTexture;           

struct VS_OUTPUT_SKYBOX
{
    float4 Position   : SV_Position;   
    float4 Diffuse    : COLOR0;        
    float3 TextureUV  : TEXCOORD0;     //Dreidimensionale Texturkoordinaten!
};

//--------------------------------------------------------------------------------------
// Struktur für Rendern in Cubemap
//--------------------------------------------------------------------------------------
struct GS_OUTPUT_SKYBOXTOCUBEMAP
{
    float4 Position   : SV_POSITION;     
    float4 Diffuse    : COLOR0;
    float3 TextureUV  : TEXCOORD0;       
    uint RTIndex      : SV_RenderTargetArrayIndex;
};

//--------------------------------------------------------------------------------------
// Vertexshader für Skybox
//--------------------------------------------------------------------------------------
VS_OUTPUT_SKYBOX RenderSkyBoxVS(float4 vPos : POSITION, 
                                float3 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT_SKYBOX Output = (VS_OUTPUT_SKYBOX)0.0f;
    Output.Position = mul(vPos, g_mWorldViewProjection);
    
    //volle Helligkeit
    Output.Diffuse = (float4)1.0f; 
    Output.TextureUV = vTexCoord0; 
    
    return Output;    
}

VS_OUTPUT_SKYBOX RenderSkyBoxToCubeMapVS(float4 vPos : POSITION, 
                                  float3 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT_SKYBOX Output = (VS_OUTPUT_SKYBOX)0.0f;
    Output.Position = mul(vPos, g_mWorld);
    
    //volle Helligkeit
    Output.Diffuse = (float4)1.0f; 
    Output.TextureUV = vTexCoord0;
    
    return Output;
}

[maxvertexcount(18)]
void RenderSkyBoxToCubeMapGS( triangle VS_OUTPUT_SKYBOX input[3], 
    inout TriangleStream<GS_OUTPUT_SKYBOXTOCUBEMAP> CubeMapStream )
{
    for( int f = 0; f < 6; ++f )
    {
        GS_OUTPUT_SKYBOXTOCUBEMAP Output = (GS_OUTPUT_SKYBOXTOCUBEMAP) 0;
        Output.RTIndex = f;  //0 = rechts, 1 = links, 2 = oben
                             //3 = unten, 4 = vorn, 5 = hinten 
        for( int v = 0; v < 3; v++ )
        {
            Output.Position  = mul(input[v].Position, g_mViewCM[f] );
            Output.Position  = mul(Output.Position, g_mProj );
            Output.Diffuse   = input[v].Diffuse;
            Output.TextureUV = input[v].TextureUV;
            CubeMapStream.Append(Output);
        }
        CubeMapStream.RestartStrip();
    }
}
//--------------------------------------------------------------------------------------
// Pixelshader für Skybox
//--------------------------------------------------------------------------------------
float4 RenderSkyBoxPS(VS_OUTPUT_SKYBOX In) : SV_Target
{ 
    return g_SkyboxTexture.Sample(g_samWrap, In.TextureUV);// * In.Diffuse;
}

//--------------------------------------------------------------------------------------
// Pixelshader für Rendern in Cubemap
//--------------------------------------------------------------------------------------
float4 RenderSkyBoxToCubeMapPS(GS_OUTPUT_SKYBOXTOCUBEMAP In) : SV_Target
{
    return g_SkyboxTexture.Sample(g_samWrap, In.TextureUV);// * In.Diffuse;
}
//--------------------------------------------------------------------------------------
// techniques
//--------------------------------------------------------------------------------------
technique11 RenderSkyBox
{
    pass P0
    {       
		SetVertexShader( CompileShader(vs_4_0, RenderSkyBoxVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, RenderSkyBoxPS() ) );
        
        //SetDepthStencilState( DisableDepthWrite, 0 );
    }
}

technique11 RenderToCubeMap
{
    pass P0
    {       
		SetVertexShader( CompileShader(vs_4_0, RenderSkyBoxToCubeMapVS()));
        SetGeometryShader( CompileShader(gs_4_0, RenderSkyBoxToCubeMapGS()));
        SetPixelShader( CompileShader(ps_4_0, RenderSkyBoxToCubeMapPS()));
        
        //SetDepthStencilState(DisableDepthWrite, 0);
    }
};

