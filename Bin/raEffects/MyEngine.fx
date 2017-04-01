//--------------------------------------------------------------------------------------
// File: MyEngine.fx
//
// Effekte für die Engine
//
//--------------------------------------------------------------------------------------

#include "MyEngine.fxh"
#include "Cubemap.fxh" 

//--------------------------------------------------------------------------------------
// Einfache Shader
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Vertexshader mit Vertexfarbe
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderWithColorVS(float4 vPos : POSITION, 
                            float3 color : COLOR0)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);
    
    Output.Diffuse.rgb = color;   
    Output.Diffuse.a = 1.0f; 
    
    return Output;    
}

//--------------------------------------------------------------------------------------
// Rendern mit Vertexfarbe
//--------------------------------------------------------------------------------------
technique11 RenderWithColor
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWithColorVS()));
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0, SimplePS()));
    }
}

//--------------------------------------------------------------------------------------
// Rendern mit Textur
// Muss in MyEngine.fx bleiben wegen Beispielen ohne Effektpool
// Auch in Kap13_01 verwendet
//--------------------------------------------------------------------------------------
technique11 RenderWithTexture
{
    pass P0
    {       
		SetVertexShader( CompileShader(vs_4_0, RenderWithTextureVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, RenderSceneFullBrightPS() ) );
    }
}

//--------------------------------------------------------------------------------------
// Rendern mit Textur in der Bildschirmmitte
// Vgl. Auch Postprocess
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderWithTextureCenterScreenVS(uint VertexId: SV_VertexID)
{
    float size = 0.1f;

    VS_OUTPUT Output = (VS_OUTPUT)0;
    
	Output.Position = float4(size * QuadPositionTexCoords[VertexId].xy, 0, 1);
    
    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse + 
        g_MaterialAmbientColor;   
    Output.Diffuse.a = 1.0f; 
    
	Output.TextureUV = QuadPositionTexCoords[VertexId].zw;
    
    return Output;
}

technique11 RenderWithTextureCenterScreen
{
    pass P0
    {       
		SetVertexShader( CompileShader(vs_4_0, RenderWithTextureCenterScreenVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, RenderSceneFullBrightPS() ) );
    }
}

//--------------------------------------------------------------------------------------
// Rendern ohne Textur, mit Richtungslicht, feste Farbe
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderWithMaterialColorVS(float4 vPos       : POSITION, 
                                    float3 vNormal    : NORMAL)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);

    float3 vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); 
    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * 
                         max(0,dot(vNormalWorldSpace, g_vLightDir)) + 
                         g_MaterialAmbientColor;   
    Output.Diffuse.a = 1.0f; 
    
    return Output;    
}

technique11 RenderWithMaterialColor
{
    pass P0
    {       
        SetVertexShader( CompileShader(vs_4_0, RenderWithMaterialColorVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, SimplePS() ) );
    }
}

//--------------------------------------------------------------------------------------
// Rendern in Weiss
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderWhiteVS(float4 vPos : POSITION)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);
    
    Output.Diffuse = (float4) 1.0f;   
    
    return Output;    
}

technique11 RenderWhite
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWhiteVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, SimplePS()));
    }
}

//Das hier muss in alle Effektdateien, die in Cubemap rendern!
technique11 RenderToCubeMap
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, VS_CubeMapDirectional()));
        SetGeometryShader(CompileShader(gs_4_0, GS_CubeMap()));
        SetPixelShader(CompileShader(ps_4_0, PS_CubeMap()));
    }
};


//--------------------------------------------------------------------------------------
// Shader aus SimpleSample - hier nur aufgenommen, damit man in Kapitel 05 bis 08 mit 
// einer einzigen Effektdatei auskommt für Entity, SkyDome, Triangle, Quad etc. , 
// Objekte ohne Textur (RenderWithMaterialColor)
// Pixelshader ist bereits in der .fxh-Datei
//--------------------------------------------------------------------------------------

VS_OUTPUT RenderSceneVS( float4 vPos : POSITION, 
                         float3 vNormal : NORMAL,
                         float2 vTexCoord0 : TEXCOORD0 )
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);
    
    vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); // normal (world space)

    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * max(0,dot(vNormalWorldSpace, g_vLightDir)) + 
                         g_MaterialAmbientColor;   
    Output.Diffuse.a = 1.0f; 
    
    Output.TextureUV = vTexCoord0; 
    
    return Output;    
}

technique11 RenderScene
{
    pass P0
    {       
		SetVertexShader( CompileShader(vs_4_0, RenderSceneVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, RenderScenePS() ) );
    }
}

technique11 RenderToLightMap
{
    pass P0
    {       
		SetVertexShader( CompileShader(vs_4_0, RenderToLightMapVS()));
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, RenderToLightMapPS()));
    }
}
