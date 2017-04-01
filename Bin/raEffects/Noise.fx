#include "MyEngine.fxh"
#include "Noise.fxh"
//--------------------------------------------------------------------------------------
// File: Noise.fx
//
// Procedurale Geometrie und Textur
//
//--------------------------------------------------------------------------------------

DepthStencilState EnableDepthWrite
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

VS_OUTPUT ProceduralVS(float4 vPos : POSITION,
                           float2 vTexCoord0 : TEXCOORD0, 
                       uniform bool useHeight)
{
    VS_OUTPUT Output = (VS_OUTPUT) 0;
    Output.Position = vPos;
    if(useHeight) Output.Position.y += fbm(vTexCoord0.x, vTexCoord0.y);

    Output.Position = mul(Output.Position, g_mWorldViewProjection);
    Output.TextureUV = vTexCoord0;
    return Output;    
}

VS_OUTPUT_PERPIXELLIGHTING ProceduralLightedVS( float4 vPos: POSITION, 
                                 float3 vNormal    : NORMAL,
                                 float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT_PERPIXELLIGHTING Output = (VS_OUTPUT_PERPIXELLIGHTING)0;
    
    Output.Position = vPos;
    Output.Position.y += fbm(vTexCoord0.x, vTexCoord0.y);
    Output.WorldPos = mul(Output.Position, (float3x3)g_mWorld);
 
    Output.Position = mul(Output.Position, g_mWorldViewProjection);
    Output.TextureUV = vTexCoord0;
    return Output;    
}

/*
VS_OUTPUT ProceduralLightedVS2(float4 vPos: POSITION, 
                               float3 vNormal    : NORMAL,
                               float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
    Output.Position = vPos;
    Output.Position.y += fbm(vTexCoord0.x, vTexCoord0.y);
 
	float4 n = FindFbmNormal(vTexCoord0); 
    float3 vNormalWorldSpace = normalize(mul(n, (float3x3)g_mWorld)); 

    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * 
                         max(0,dot(vNormalWorldSpace, g_vLightDir)) + 
                         g_MaterialAmbientColor;   
    Output.Diffuse.a = 1.0f; 

    Output.Position = mul(Output.Position, g_mWorldViewProjection);
    Output.TextureUV = vTexCoord0;
    return Output;    
}

VS_OUTPUT ProceduralLightedVS3(float4 vPos: POSITION, 
                               float3 vNormal    : NORMAL,
                               float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
    Output.Position = vPos;
    Output.Position.y += fbm(vTexCoord0.x, vTexCoord0.y);
 
    //Normale wird komplett im Pixelshader berechnet
    Output.Diffuse = (float4)1; 

    Output.Position = mul(Output.Position, g_mWorldViewProjection);
    Output.TextureUV = vTexCoord0;
    return Output;    
}
*/
float4 PerlinPS(VS_OUTPUT In) : SV_Target
{ 
    float s = h(g_freq * In.TextureUV.x, g_freq * In.TextureUV.y);

    float4 color = (float4) (2.0f * s + g_bias); 
    color.a = 1.0f;
	
    return color;
}

float4 FbmPS(VS_OUTPUT In, uniform bool useHeightTexture) : SV_Target
{ 
    float s = fbm(In.TextureUV.x, In.TextureUV.y);
    
    //Kachelfähig (für Wolkentextur!)
    //float s = 
    //  (1 - f(In.TextureUV.x)) * fbm(In.TextureUV.x, In.TextureUV.y)
    //+ f(In.TextureUV.x) * fbm(1-In.TextureUV.x, In.TextureUV.y);
    
    float4 color; 
    if(useHeightTexture)
        color = g_txColorsFromHeight.Sample(g_samWrap, g_scale * s - g_bias);// * g_MaterialAmbientColor;
    else
        //color = (float4) s;
        color = (float4) (0.5f * s + g_bias); 
    color.a = 1.0f;
    return color;
}
/*
float4 FbmLightedPS1(VS_OUTPUT_PERPIXELLIGHTING In) : SV_Target
{ 
    float3 normal = FindNormal(In.WorldPos);
    float s = fbm(In.TextureUV.x, In.TextureUV.y);

    float4 color = g_txColorsFromHeight.Sample(g_samWrap, g_scale * s - g_bias) *
        (g_MaterialDiffuseColor * g_LightDiffuse * max(0,dot(normal, g_vLightDir)) + 
         g_MaterialAmbientColor);

    color.a = 1.0f;
    return color;
}

float4 FbmLightedPS2(VS_OUTPUT In) : SV_Target
{ 
    float s = fbm(In.TextureUV.x, In.TextureUV.y);

    float4 color = In.Diffuse * g_txColorsFromHeight.Sample(g_samWrap, g_scale * s - g_bias);// * g_MaterialAmbientColor;
    color.a = 1.0f;
    return color;
}
*/
float4 FbmLightedPS(VS_OUTPUT In) : SV_Target
{ 
    float s = fbm(In.TextureUV.x, In.TextureUV.y);
    float4 n = FindFbmNormal(In.TextureUV);
    
    float3 vNormalWorldSpace = normalize(mul(n, (float3x3)g_mWorld)); 
    
    float4 color = g_txColorsFromHeight.Sample(g_samWrap, 
        g_scale * s - g_bias) * (g_MaterialDiffuseColor * g_LightDiffuse * 
        max(0,dot(vNormalWorldSpace, g_vLightDir)) + g_MaterialAmbientColor);
    color.a = 1.0f;
    return color;
}

float4 FbmNormalsPS(VS_OUTPUT In) : SV_Target
{ 
    float3 normal = FindFbmNormal(In.TextureUV);
    return float4(normal, 1);
}

technique11 Perlin
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, ProceduralVS(true)));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PerlinPS()));
        
        SetDepthStencilState(EnableDepthWrite, 0);
   }
};
/*
technique11 FbmLighted1
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, ProceduralLightedVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, FbmLightedPS1()));
        
        SetDepthStencilState(EnableDepthWrite, 0);
    }
};

technique11 FbmLighted2
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, ProceduralLightedVS2()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, FbmLightedPS2()));
        
        SetDepthStencilState(EnableDepthWrite, 0);
    }
};
*/
technique11 FbmLighted
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, ProceduralVS(true)));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, FbmLightedPS()));
        
        SetDepthStencilState(EnableDepthWrite, 0);
    }
};

technique11 FbmWithHeight
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, ProceduralVS(true)));
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0, FbmPS(true)));
        
        //SetDepthStencilState(EnableDepth, 0);
    }
};

technique11 FbmGrey
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, ProceduralVS(false)));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, FbmPS(false)));
    }
}

technique11 PerlinNoHeight
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, ProceduralVS(false)));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PerlinPS()));
    }
}

technique11 Fbm
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, ProceduralVS(false)));
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0, FbmPS(true)));
    }
}

technique11 PerlinFullScreen
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, RenderWithTextureFullScreenVS()));
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0, PerlinPS()));
    }
}

technique11 FbmFullScreen
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, RenderWithTextureFullScreenVS()));
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0, FbmPS(true)));
    }
}

technique11 FbmFullScreenGrey
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, RenderWithTextureFullScreenVS()));
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0, FbmPS(false)));
    }
}

technique11 FbmNormalsFullScreen
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, RenderWithTextureFullScreenVS()));
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0, FbmNormalsPS()));
    }
}