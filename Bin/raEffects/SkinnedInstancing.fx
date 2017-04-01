//--------------------------------------------------------------------------------------
// File: SkinnedInstancing.fx
//
// Instancing für Modelle mit Skelettbasierter Animation
//
// 
//--------------------------------------------------------------------------------------
#include "MyEngine.fxh"

struct VSInstIn
{
    float4 vPos     : POSITION;
    float4 vWeights : WEIGHTS;    //Gewichtsfaktoren
    uint4  vBones   : BONES;	  //Knochennummern
    float3 vNormal  : NORMAL;
    float2 tex      : TEXCOORD0;
    row_major float4x4 mTransform : mTransform;
    uint   InstanceId   : SV_InstanceID;
};

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

VS_OUTPUT RenderInstancedSkinnedVS(VSInstIn input)
{
    VS_OUTPUT Output = (VS_OUTPUT) 0;

    uint key = (uint)(15 * g_fTime);
    //key = key % (g_NumKeys - 1) + g_StartKey;
    key = key % (g_NumKeysArray[input.InstanceId] - 1)
        + g_StartKeyArray[input.InstanceId];
        
    float4x4 finalMatrix = (float4x4) 0;
    finalMatrix  = input.vWeights.x * loadBoneMatrix(key, input.vBones.x);
    finalMatrix += input.vWeights.y * loadBoneMatrix(key, input.vBones.y);
    finalMatrix += input.vWeights.z * loadBoneMatrix(key, input.vBones.z);
    finalMatrix += input.vWeights.w * loadBoneMatrix(key, input.vBones.w);
    
    float4 AnimatedPosition = mul(input.vPos, finalMatrix);
    float4 AnimatedNormal   = mul(float4(input.vNormal.xyz,0),finalMatrix);
    
    float4 InstancePosition = mul(AnimatedPosition, input.mTransform);
    
    float3 vNormalWorldSpace = 
        normalize(mul(AnimatedNormal.xyz, (float3x3)input.mTransform));
    
    Output.Position = mul(InstancePosition, g_mView);
    Output.Position = mul(Output.Position, g_mProj);
    
    //Wie RenderDirectional
    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * 
        max(0,dot(vNormalWorldSpace, g_vLightDir)) +
        g_MaterialAmbientColor * g_LightAmbientColor;      
    Output.Diffuse.a = 1.0f; 
    
    Output.TextureUV = input.tex;
    
    return Output;
}

technique11 RenderInstancedSkinned
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, RenderInstancedSkinnedVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, RenderScenePS()));
   }  
}

technique11 RenderInstancedSkinnedNoTex
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, RenderInstancedSkinnedVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, SimplePS()));
    }  
}



