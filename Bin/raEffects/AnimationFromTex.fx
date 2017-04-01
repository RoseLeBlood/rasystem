//--------------------------------------------------------------------------------------
// File: AnimationFromTex.fx
//
// Animation im Shader
//
// 
//--------------------------------------------------------------------------------------

#include "MyEngine.fxh"

//--------------------------------------------------------------------------------------
// Globale Variablen
//--------------------------------------------------------------------------------------

float4x4 loadBoneMatrix(uint key, uint bone)
{
    uint baseU = 4 * bone;
    uint baseV = key;
   
    float4 mat1 = g_txAnimations.Load(uint3(baseU    , baseV,0));
    float4 mat2 = g_txAnimations.Load(uint3(baseU + 1, baseV,0));
    float4 mat3 = g_txAnimations.Load(uint3(baseU + 2, baseV,0));
    float4 mat4 = g_txAnimations.Load(uint3(baseU + 3, baseV,0));
      
    return float4x4(mat1,mat2,mat3, mat4);
}

//--------------------------------------------------------------------------------------
// Vertexshader für Animation
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderAnimationVS( float4 vPos       : POSITION, 
                             float4 Weights    : WEIGHTS, //Gewichtsfaktoren
                             uint4  vBones      : BONES,	  //Knochennummern
                             float3 vNormal    : NORMAL,
                             float2 vTexCoord0 : TEXCOORD0,
                             float3 vTangent   : TANGENT)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;

    uint key = (uint)(15 * g_fTime);
    key = key % (g_NumKeys - 1) + g_StartKey;

    matrix m0 = loadBoneMatrix(key, vBones.x);
    matrix m1 = loadBoneMatrix(key, vBones.y);
    matrix m2 = loadBoneMatrix(key, vBones.z);
    matrix m3 = loadBoneMatrix(key, vBones.w);
	
    Output.Position += Weights.x * mul(vPos, m0);
    Output.Position += Weights.y * mul(vPos, m1);
    Output.Position += Weights.z * mul(vPos, m2);
    Output.Position += Weights.w * mul(vPos, m3);

    Output.Position = mul(Output.Position, g_mWorldViewProjection); 
    
    // Beleuchtung vereinfacht:
    // Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse + 
    //    g_MaterialAmbientColor;   
    // Output.Diffuse.a = 1.0f; 
    
    float3 vNormalWorldSpace = (float3) 0;
    vNormalWorldSpace += Weights.x * mul(vNormal, (float3x3)m0); 
    vNormalWorldSpace += Weights.y * mul(vNormal, (float3x3)m1); 
    vNormalWorldSpace += Weights.z * mul(vNormal, (float3x3)m2); 
    vNormalWorldSpace += Weights.w * mul(vNormal, (float3x3)m3); 
    vNormalWorldSpace = mul(vNormalWorldSpace, g_mWorld); 
    vNormalWorldSpace = normalize(vNormalWorldSpace); 

    Output.Diffuse.rgb = g_MaterialDiffuseColor.rgb * g_LightDiffuse.rgb * 
        max(0,dot(vNormalWorldSpace, g_vLightDir)) + 
        g_MaterialAmbientColor.rgb * g_LightAmbientColor.rgb;   
    Output.Diffuse.a = 1.0f; 
    
    Output.TextureUV = vTexCoord0; 
    
    return Output;
}

VS_OUTPUT RenderAnimationVSSelectable( float4 vPos       : POSITION, 
                             float4 Weights    : WEIGHTS, //Gewichtsfaktoren
                             uint4  vBones     : BONES,	  //Knochennummern
                             float3 vNormal    : NORMAL,
                             float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
	uint key = (uint)(15 * g_fTime);
	key = key % (2000 - 1);
	//uint key = 2; 
    Output.Position += Weights.x * mul(vPos, loadBoneMatrix(key, vBones.x));
    Output.Position += Weights.y * mul(vPos, loadBoneMatrix(key, vBones.y));
    Output.Position += Weights.z * mul(vPos, loadBoneMatrix(key, vBones.z));
    Output.Position += Weights.w * mul(vPos, loadBoneMatrix(key, vBones.w));

    Output.Position = mul(Output.Position, g_mView);
    Output.Position = mul(Output.Position, g_mProj);

    float3 vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); 

    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * 
        max(0,dot(vNormalWorldSpace, g_vLightDir)) + 
        g_MaterialAmbientColor.rgb * g_LightAmbientColor.rgb;   
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