//--------------------------------------------------------------------------------------
// File: DeferredLighting.fx
//
// Deferred Lighting
//
// 
//--------------------------------------------------------------------------------------

#include "MyEngine.fxh"
#include "Noise.fxh"

Texture2DArray g_DeferredTexture; 

//--------------------------------------------------------------------------------------
// Rendern in Textur
//--------------------------------------------------------------------------------------
struct VS_OUTPUT_DEFERRED
{
    float4 Position             : SV_Position;
    float4 Diffuse              : COLOR0;     
    float4 PositionViewSpace    : POSITIONT;     
    float4 NormalViewSpace      : NORMALT;  
    float2 TextureUV            : TEXCOORD0;  
    float4 PositionFromLight    : TEXCOORD1; 
};

VS_OUTPUT_DEFERRED VSDeferred(
    float4 vPos       : POSITION, 
    float3 vNormal    : NORMAL,
    float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT_DEFERRED Output = (VS_OUTPUT_DEFERRED) 0;
    Output.Position = mul(vPos, g_mWorldViewProjection);
    
    //WorldView im C++-Code einmal pro Objekt vorzumultiplizieren ist effizienter als es hier einmal pro Vertex zu tun
    //float4 vPosWorldSpace = mul(vPos, g_mWorld);
    //Output.PositionViewSpace = mul(vPosWorldSpace, g_mView);
	Output.PositionViewSpace = mul(vPos, g_mWorldView);                  
	
    //float3 vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld));
    //Output.NormalViewSpace.xyz = mul(vNormalWorldSpace, (float3x3)g_mView);
	Output.NormalViewSpace.xyz = mul(vNormal, (float3x3)g_mWorldView);
   
    Output.Diffuse = (float4) 1.0f;
    
    Output.TextureUV = vTexCoord0; 
    Output.PositionFromLight = mul(vPos, g_mLightViewProjection);
    return Output;    
}

VS_OUTPUT_DEFERRED VSDeferredNoNormals(
    float4 vPos       : POSITION, 
    float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT_DEFERRED Output = (VS_OUTPUT_DEFERRED) 0;
    float4 vPosWorldSpace = mul(vPos, g_mWorld);
    Output.Position = mul(vPos, g_mWorldViewProjection);
    Output.PositionViewSpace = mul(vPosWorldSpace, g_mView);
   
    Output.Diffuse = (float4) 1.0f;
    
    Output.TextureUV = vTexCoord0; 
    Output.PositionFromLight = mul(vPos, g_mLightViewProjection);
    return Output;    
}

//Die Normalen müssen auch hier eigentlich aus der Steigung der Positionsdaten berechnet werden!
VS_OUTPUT_DEFERRED VSDeferredProcedural(
    float4 vPos       : POSITION, 
    float3 vNormal    : NORMAL,
    float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT_DEFERRED Output = (VS_OUTPUT_DEFERRED) 0;
    float4 vPos1 = vPos;
    vPos1.y += fbm(vTexCoord0.x, vTexCoord0.y);
    Output.Position = mul(vPos1, g_mWorldViewProjection);
    
	Output.PositionViewSpace = mul(vPos1, g_mWorldView);                  
	Output.NormalViewSpace.xyz = mul(vNormal, (float3x3)g_mWorldView);
   
    Output.Diffuse = (float4) 1.0f;
    
    Output.TextureUV = vTexCoord0; 
    Output.PositionFromLight = mul(vPos1, g_mLightViewProjection);
    return Output;    
}

[maxvertexcount(12)]
void GSDeferred(triangle VS_OUTPUT_DEFERRED input[3], 
    inout TriangleStream<PS_CUBEMAP_IN> Stream)
{
    for(int f = 0; f < 4; ++f )
    {
        PS_CUBEMAP_IN Output = (PS_CUBEMAP_IN) 0;
        for(int v = 0; v < 3; v++ )
        {
            Output.RTIndex = f;  
            Output.Position = input[v].Position;  
            Output.TextureUV = input[v].TextureUV;  
            switch(f)
            {
                case 0:
                    Output.Diffuse = input[v].Diffuse;
                    break;              
                case 1:
                    Output.Diffuse = input[v].PositionViewSpace;
                    break;                  
                case 2:
                    Output.Diffuse = input[v].NormalViewSpace;
                    break;              
                case 3: //Für das Rendern mit Lightmap
                    Output.Diffuse = input[v].PositionFromLight; 
					break;
                case 4: //In die Lightmap rendern 
                    Output.Position = input[v].PositionFromLight;
                    Output.TextureUV = input[v].PositionFromLight.zw;
                    //Diffuse nicht gebraucht
                    break;              
            } 
            Stream.Append(Output);
        }
        Stream.RestartStrip();
    }
}

float4 PSDeferredProcedural(PS_CUBEMAP_IN In) : SV_Target
{ 
    //Anmerkung: Sehr schöne Effekte ergeben sich, wenn man auch Position und Normale mit Noise bearbeitet
    if(In.RTIndex == 0)
    {
        float s = fbm(In.TextureUV.x, In.TextureUV.y);

        float4 color = In.Diffuse * g_txColorsFromHeight.Sample(g_samWrap, g_scale * s - g_bias);// * g_MaterialAmbientColor;
        color.a = 1.0f;
        return color;
    }
    else if(In.RTIndex == 4) //Rendern in die Lightmap
    {
        return (float4) In.TextureUV.x / In.TextureUV.y; 
    }
    else return In.Diffuse;
}

float4 PSDeferred(PS_CUBEMAP_IN In) : SV_Target
{
    if(In.RTIndex == 0)
        return g_MeshTexture.Sample(g_samWrap, In.TextureUV) * In.Diffuse;
    else 
        return In.Diffuse;
}

float4 PSDeferredNoTex(PS_CUBEMAP_IN In) : SV_Target
{
    return In.Diffuse;
}

technique11 RenderDeferredNoTex
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, VSDeferred()));
        SetGeometryShader(CompileShader(gs_4_0, GSDeferred()));
        SetPixelShader(CompileShader(ps_4_0, PSDeferredNoTex()));
    }
};

technique11 RenderDeferred
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, VSDeferred()));
        SetGeometryShader(CompileShader(gs_4_0, GSDeferred()));
        SetPixelShader(CompileShader(ps_4_0, PSDeferred()));
    }
};

technique11 RenderDeferredProcedural
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, VSDeferredProcedural()));
        SetGeometryShader(CompileShader(gs_4_0, GSDeferred()));
        SetPixelShader(CompileShader(ps_4_0, PSDeferredProcedural()));
    }
};

technique11 RenderDeferredNoNormals
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, VSDeferredNoNormals()));
        SetGeometryShader(CompileShader(gs_4_0, GSDeferred()));
        SetPixelShader(CompileShader(ps_4_0, PSDeferred()));
    }
};
//--------------------------------------------------------------------------------------
// Richtungslicht
//--------------------------------------------------------------------------------------
float DirectionalLight(float3 NormalViewSpace, 
    float3 LightDirViewSpace)
{
    float3 NViewSpace = normalize(NormalViewSpace);  
    return max(0,dot(NViewSpace, LightDirViewSpace));
}

float4 PSDirectionalDeferred(VS_OUTPUT In) : SV_Target
{ 
    float3 texco0 = float3(In.TextureUV, 0); 
    float4 Diffuse = g_DeferredTexture.Sample(g_samWrap, texco0);
    
    float3 texco1 = float3(In.TextureUV, 1); 
    float4 PositionViewSpace = g_DeferredTexture.Sample(g_samWrap, texco1);

    float3 texco2 = float3(In.TextureUV, 2); 
    float4 NormalViewSpace = g_DeferredTexture.Sample(g_samWrap, texco2);
    
    float3 texco3 = float3(In.TextureUV, 3); 
    float4 f = g_DeferredTexture.Sample(g_samWrap, texco3);
    float2 TextureUV = f.xy;
    float2 PositionFromLight = f.zw;

    return Diffuse  * (
        DirectionalLight(NormalViewSpace.xyz, -g_LightDirViewSpace) * 
        g_MaterialDiffuseColor * g_LightDiffuse +
        g_MaterialAmbientColor * g_LightAmbientColor);
}

technique11 DirectionalDeferred
{
    pass P0
    {       
        SetVertexShader(CompileShader(vs_4_0, RenderWithTextureVS()));
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0,  PSDirectionalDeferred()));
    }
}

technique11 DirectionalDeferredFullScreen
{
    pass P0
    {       
        SetVertexShader( CompileShader(vs_4_0, RenderWithTextureFullScreenVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, PSDirectionalDeferred() ) );
    }
}
//--------------------------------------------------------------------------------------
// Punktlicht
//--------------------------------------------------------------------------------------
float PointLight(float4 PositionViewSpace, float3 NormalViewSpace, 
    float4 LightPosViewSpace, float4 LightAttenuation)
{
    float3 NViewSpace = normalize(NormalViewSpace); 
    float3 toLight = LightPosViewSpace.xyz - PositionViewSpace.xyz;
    float  lightDist = length(toLight);
    float  fAtten = 1.0/dot(LightAttenuation, 
        float4(1, lightDist, lightDist*lightDist,0));
    float3 LViewSpace = toLight / lightDist; 

    return max(0,dot(NViewSpace, LViewSpace)) * fAtten; 
}

float4 PSPointLightDeferred(VS_OUTPUT In) : SV_Target
{ 
    float3 texco0 = float3(In.TextureUV, 0); 
    float4 Diffuse = g_DeferredTexture.Sample(g_samWrap, texco0);
    
    float3 texco1 = float3(In.TextureUV, 1); 
    float4 PositionViewSpace = g_DeferredTexture.Sample(g_samWrap, texco1);

    float3 texco2 = float3(In.TextureUV, 2); 
    float4 NormalViewSpace = g_DeferredTexture.Sample(g_samWrap, texco2);
    
    float3 texco3 = float3(In.TextureUV, 3); 
    float4 f = g_DeferredTexture.Sample(g_samWrap, texco3);
    float2 TextureUV = f.xy;
    float2 PositionFromLight = f.zw;
    
    return Diffuse * (
        PointLight(PositionViewSpace, NormalViewSpace.xyz, 
        g_LightPosViewSpace, g_LightAttenuation) * 
        g_MaterialDiffuseColor * g_LightDiffuse +
        g_MaterialAmbientColor * g_LightAmbientColor);
}

//--------------------------------------------------------------------------------------
// Spotlicht
//--------------------------------------------------------------------------------------
float SpotLight(float4 PositionViewSpace, float3 NormalViewSpace, 
    float4 LightPosViewSpace, float3 LightDirViewSpace)
{
    float3 NViewSpace = normalize(NormalViewSpace); 
    float3 toLight = g_LightPosViewSpace.xyz - PositionViewSpace.xyz;
    float3 LViewSpace = normalize(toLight);
    
    float cosAlpha = dot(LViewSpace, LightDirViewSpace);
    float spotEffect = smoothstep(g_cosHalfPhi, g_cosHalfTheta, cosAlpha); 
    
    return max(0, dot(NViewSpace, LViewSpace)) * spotEffect;
}

float4 PSSpotLightDeferred(VS_OUTPUT In) : SV_Target
{ 
    float3 texco0 = float3(In.TextureUV, 0); 
    float4 Diffuse = g_DeferredTexture.Sample(g_samWrap, texco0);
    
    float3 texco1 = float3(In.TextureUV, 1); 
    float4 PositionViewSpace = g_DeferredTexture.Sample(g_samWrap, texco1);

    float3 texco2 = float3(In.TextureUV, 2); 
    float4 NormalViewSpace = g_DeferredTexture.Sample(g_samWrap, texco2);
    
    float3 texco3 = float3(In.TextureUV, 3); 
    float4 f = g_DeferredTexture.Sample(g_samWrap, texco3);
    float2 TextureUV = f.xy;
    float2 PositionFromLight = f.zw;
    
    return Diffuse * (
        SpotLight(PositionViewSpace, NormalViewSpace.xyz, 
        g_LightPosViewSpace, -g_LightDirViewSpace) * 
        g_MaterialDiffuseColor * g_LightDiffuse +
        g_MaterialAmbientColor * g_LightAmbientColor);
}

//--------------------------------------------------------------------------------------
// Mehrere Lichtquellen
//--------------------------------------------------------------------------------------
float4 PSMultiLightDeferred(VS_OUTPUT In) : SV_Target
{ 
    float3 texco0 = float3(In.TextureUV, 0); 
    float4 Diffuse = g_DeferredTexture.Sample(g_samWrap, texco0);
    
    float3 texco1 = float3(In.TextureUV, 1); 
    float4 PositionViewSpace = g_DeferredTexture.Sample(g_samWrap, texco1);

    float3 texco2 = float3(In.TextureUV, 2); 
    float4 NormalViewSpace = g_DeferredTexture.Sample(g_samWrap, texco2);
    
    float3 texco3 = float3(In.TextureUV, 3); 
    float4 f = g_DeferredTexture.Sample(g_samWrap, texco3);
    float2 TextureUV = f.xy;
    float2 PositionFromLight = f.zw;

    return Diffuse * ((PointLight(PositionViewSpace, NormalViewSpace.xyz, 
        g_Lights[0].PosViewSpace, g_Lights[0].Attenuation)   
        + SpotLight(PositionViewSpace, NormalViewSpace.xyz, 
        g_Lights[1].PosViewSpace, -g_Lights[1].DirViewSpace)) * 
        g_MaterialDiffuseColor * g_LightDiffuse + g_MaterialAmbientColor * 
        (g_Lights[0].Ambient + g_Lights[1].Ambient));
}

technique11 PointLightDeferred
{
    pass P0
    {       
        SetVertexShader(CompileShader(vs_4_0, RenderWithTextureVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0,  PSPointLightDeferred()));
    }
}

technique11 PointLightDeferredFullScreen
{
    pass P0
    {       
        SetVertexShader(CompileShader(vs_4_0, RenderWithTextureFullScreenVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PSPointLightDeferred()));
    }
}

technique11 SpotLightDeferredFullScreen
{
    pass P0
    {       
        SetVertexShader(CompileShader(vs_4_0, RenderWithTextureFullScreenVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PSSpotLightDeferred()));
    }
}

technique11 MultiLightDeferredFullScreen
{
    pass P0
    {       
        SetVertexShader( CompileShader(vs_4_0, RenderWithTextureFullScreenVS()));
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, PSMultiLightDeferred()));
    }
}

