//--------------------------------------------------------------------------------------
// File: HeightmapEffect.fx
//
// Höhe aus Textur, Richtungslicht
//
//--------------------------------------------------------------------------------------

#include "MyEngine.fxh"
float g_d = 1.0f / 1024; //0.025f;
float4 FindHeightTexNormal(float2 vTexCoord0)
{
    float2 tex1 = float2(vTexCoord0.x + g_d, vTexCoord0.y); 
    float2 tex2 = float2(vTexCoord0.x, vTexCoord0.y + g_d); 
    float s0 = g_HeightTexture.SampleLevel(g_samWrap, vTexCoord0, 0).r; 
    float s1 = g_HeightTexture.SampleLevel(g_samWrap, tex1, 0).r; 
    float s2 = g_HeightTexture.SampleLevel(g_samWrap, tex2, 0).r; 
	return normalize(float4(s0 - s1, s0 - s2, g_d, 0)); 
}

//--------------------------------------------------------------------------------------
// Vertexshader
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderWithHeightMapVS( float4 vPos       : POSITION, 
                                 //float3 vNormal    : NORMAL,
                                 float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    Output.Position = vPos;
    
    //Weil beim Speichern der Höhenwerte 1 addiert werden muss, hier wieder abziehen ??
    //Wie CPU-Terrain
    Output.Position.y += 2 * (g_HeightTexture.SampleLevel(g_samWrap, vTexCoord0, 0).r - 0.5f);
    
    Output.Position = mul(Output.Position, g_mWorldViewProjection);

    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse + g_MaterialAmbientColor;   
    Output.Diffuse.a = 1.0f; 
    
    Output.TextureUV = vTexCoord0; 
    
    return Output;
}
/*
VS_OUTPUT RenderWithHeightMapLightedVS( 
    float4 vPos       : POSITION, 
    float3 vNormal    : NORMAL,
    float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
    Output.Position = vPos;
    Output.Position.y += g_HeightTexture.SampleLevel(g_samWrap, vTexCoord0, 0).r;
    
    Output.Position = mul(Output.Position, g_mWorldViewProjection);
    
	float3 normal = FindHeightTexNormal(vTexCoord0);
    float3 vNormalWorldSpace = normalize(mul(normal, (float3x3)g_mWorld)); 
    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * 
        max(0,dot(vNormalWorldSpace, g_vLightDir)) +
        g_MaterialAmbientColor * g_LightAmbientColor;      
    Output.Diffuse.a = 1.0f; 

    Output.TextureUV = vTexCoord0; 
    
    return Output;
}
*/
//--------------------------------------------------------------------------------------
// Pixelshader 
//--------------------------------------------------------------------------------------
float4 RenderWithHeightMapLightedPS(VS_OUTPUT In) : SV_Target
{ 
    float3 normal = FindHeightTexNormal(In.TextureUV);
    normal = normalize(mul(normal, (float3x3)g_mWorld)); 
    return g_MeshTexture.Sample(g_samWrap, In.TextureUV) *
        (g_MaterialDiffuseColor * g_LightDiffuse
         * max(0, dot(normal, g_vLightDir)) 
        + g_MaterialAmbientColor * g_LightAmbientColor);
}

float4 RenderWithHeightMapAndBumpMapPS(VS_OUTPUT In) : SV_Target
{ 
    float3 normal = g_NormalMap.Sample(g_samWrap, In.TextureUV);
    normal = normalize(mul(normal, (float3x3)g_mWorld)); 
    return g_MeshTexture.Sample(g_samWrap, In.TextureUV) *
        (g_MaterialDiffuseColor * g_LightDiffuse
         * max(0, dot(normal, g_vLightDir)) 
        + g_MaterialAmbientColor * g_LightAmbientColor);
}

/*
float4 RenderHeightMapBumpsPS(VS_OUTPUT In) : SV_Target
{ 
    return FindHeightTexNormal(In.TextureUV);
}
*/
//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 RenderWithHeightMap
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWithHeightMapVS())); //Keine Schatten, da keine Normalen
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0, RenderScenePS())); 
    }
}

technique11 RenderWithHeightMapLighted
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWithHeightMapVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, RenderWithHeightMapLightedPS())); //Pixellighting
    }
}

technique11 RenderWithHeightMapAndBumpMap
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWithHeightMapVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, RenderWithHeightMapAndBumpMapPS())); //Pixellighting
    }
}


//Nicht verwendet
/*
technique11 RenderHeightMapBumps
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWithHeightMapVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, RenderHeightMapBumpsPS())); 
    }
}
*/
