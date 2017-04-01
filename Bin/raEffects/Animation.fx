//--------------------------------------------------------------------------------------
// File: Animation.fx
//
// Animation im Shader
//
// 
//--------------------------------------------------------------------------------------
#include "MyEngine.fxh"

//--------------------------------------------------------------------------------------
// Vertexshader für Animation
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderAnimationVS( float4 vPos       : POSITION, 
                             float4 Weights    : WEIGHTS, //Gewichtsfaktoren
                             uint4  Bones      : BONES,	  //Knochennummern
                             float3 vNormal    : NORMAL,
                             float2 vTexCoord0 : TEXCOORD0,
                             float3 vTangent   : TANGENT)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
    matrix m0 = g_mConstBoneWorld[Bones.x];
    matrix m1 = g_mConstBoneWorld[Bones.y];
    matrix m2 = g_mConstBoneWorld[Bones.z];
    matrix m3 = g_mConstBoneWorld[Bones.w];
    
    Output.Position += Weights.x * mul(vPos, m0);
    Output.Position += Weights.y * mul(vPos, m1);
    Output.Position += Weights.z * mul(vPos, m2);
    Output.Position += Weights.w * mul(vPos, m3);

    Output.Position = mul(Output.Position, g_mWorldViewProjection); 
    
    // Beleuchtung vereinfacht:
    // Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse + 
    //    g_MaterialAmbientColor;   
    
    float3 vNormalWorldSpace = (float3) 0;
    vNormalWorldSpace += Weights.x * mul(vNormal, (float3x3)m0); 
    vNormalWorldSpace += Weights.y * mul(vNormal, (float3x3)m1); 
    vNormalWorldSpace += Weights.z * mul(vNormal, (float3x3)m2); 
    vNormalWorldSpace += Weights.w * mul(vNormal, (float3x3)m3); 
    vNormalWorldSpace = mul(vNormalWorldSpace, g_mWorld); 
    vNormalWorldSpace = normalize(vNormalWorldSpace); 

    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * 
        max(0,dot(vNormalWorldSpace, g_vLightDir)) + 
        g_MaterialAmbientColor * g_LightAmbientColor;   
    Output.Diffuse.a = 1.0f; 
    
    Output.TextureUV = vTexCoord0; 
    
    return Output;
}

VS_OUTPUT RenderAnimationVSSelectable( float4 vPos       : POSITION, 
                             float4 Weights    : WEIGHTS, //Gewichtsfaktoren
                             uint4  Bones      : BONES,	  //Knochennummern
                             float3 vNormal    : NORMAL,
                             float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    float3 vNormalWorldSpace;
    
    Output.Position += Weights.x * mul(vPos, g_mConstBoneWorld[Bones.x]);
    Output.Position += Weights.y * mul(vPos, g_mConstBoneWorld[Bones.y]);
    Output.Position += Weights.z * mul(vPos, g_mConstBoneWorld[Bones.z]);
    Output.Position += Weights.w * mul(vPos, g_mConstBoneWorld[Bones.w]);

    Output.Position = mul(Output.Position, g_mView);
    Output.Position = mul(Output.Position, g_mProj);

    vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); 

    Output.Diffuse = g_MaterialDiffuseColor * g_LightDiffuse * 
        max(0,dot(vNormalWorldSpace, g_vLightDir)) + 
        g_MaterialAmbientColor * g_LightAmbientColor;   
    Output.Diffuse.a = 1.0f; 
    
    if(g_bSelected) Output.Diffuse =  
        float4(0.5f, 1, 0.5f, 1) * Output.Diffuse;
    
    // Beleuchtung vereinfacht:
    // Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse + 
    //    g_MaterialAmbientColor;   
    // Output.Diffuse.a = 1.0f; 
    
    Output.TextureUV = vTexCoord0; 
    
    return Output;
}

//--------------------------------------------------------------------------------------
// techniques
//--------------------------------------------------------------------------------------
technique11 RenderAnimation
{
    pass P0
    {       
		SetVertexShader( CompileShader(vs_4_0, RenderAnimationVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, RenderScenePS()));
    }
}

technique11 RenderAnimationNoTex
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderAnimationVS()));
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0, SimplePS()));
    }
}

technique11 RenderAnimationSelectable
{
    pass P0
    {       
        SetVertexShader(CompileShader(vs_4_0, RenderAnimationVSSelectable()));
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0, RenderScenePS()));
    }
}

technique11 RenderAnimationNoTexSelectable
{
    pass P0
    {       
        SetVertexShader(CompileShader(vs_4_0, RenderAnimationVSSelectable()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, SimplePS()));
    }
}