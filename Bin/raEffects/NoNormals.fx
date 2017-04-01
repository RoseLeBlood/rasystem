//--------------------------------------------------------------------------------------
// File: NoNormals.fx
//
// Effekte für Objekte ohne Normalen (z.B. SkyBox, SkyDome)
//
// 
//--------------------------------------------------------------------------------------

#include "MyEngine.fxh"
#include "Cubemap.fxh" 

VS_OUTPUT RenderSkyDomeVS(float4 vPos       : POSITION, 
                          float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);
    
    Output.Diffuse.rgb = g_MaterialAmbientColor;   
    Output.Diffuse.a = 1.0f; 
    
    Output.TextureUV = vTexCoord0; 
    
    return Output;
}

//--------------------------------------------------------------------------------------
// Rendern mit Textur
//--------------------------------------------------------------------------------------
technique11 RenderWithTexture
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWithTextureVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, RenderSceneFullBrightPS()));
    }
}

//--------------------------------------------------------------------------------------
// Rendern mit Textur, Kein Richtungslicht, aber Ambient Light berücksichtigen 
// z.B. Skydome
//--------------------------------------------------------------------------------------
technique11 RenderWithTextureAmbient
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderSkyDomeVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, RenderScenePS()));
    }
}

//Das gibt's auch in SkyboxEffekt - SkyDome benutzt aber das hier!
technique11 RenderToCubeMap
{
    pass p0
    {
        SetVertexShader( CompileShader(vs_4_0, VS_CubeMap() ) );
        SetGeometryShader( CompileShader(gs_4_0, GS_CubeMap() ) );
        SetPixelShader( CompileShader(ps_4_0, PS_CubeMap() ) );
        
        //SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }
};

technique11 RenderToLightMap
{
    pass P0
    { 
		SetVertexShader(CompileShader(vs_4_0, RenderToLightMapVS()));  
		SetGeometryShader(NULL); 
		SetPixelShader(CompileShader(ps_4_0, RenderToLightMapPS()));        
    }
}