//--------------------------------------------------------------------------------------
// File: Instancing.fx
//
// Instancing
//
//--------------------------------------------------------------------------------------

#include "MyEngine.fxh"

RasterizerState NoCull
{
    CullMode = NONE;
};

struct VSInstIn
{
    float3 vPos    : POSITION;
    float3 vNormal : NORMAL;
    float2 tex     : TEXCOORD0;
    row_major float4x4 mTransform : mTransform;
};

struct VSInstNoNormalsIn
{
    float3 vPos    : POSITION;
    float2 tex     : TEXCOORD0;
    row_major float4x4 mTransform : mTransform;
};

VS_OUTPUT RenderInstancedVS(VSInstIn input)
{
    VS_OUTPUT Output = (VS_OUTPUT) 0;
    
    float4 InstancePosition = mul(float4(input.vPos, 1), input.mTransform);
    //Output.Position = mul(InstancePosition, g_mWorldViewProjection);
    Output.Position = mul(InstancePosition, g_mView);
    Output.Position = mul(Output.Position, g_mProj);
 
    float3 vNormalWorldSpace = 
		normalize(mul(input.vNormal, (float3x3)input.mTransform));
    
	//Wie RenderDirectional
	Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * 
		max(0,dot(vNormalWorldSpace, g_vLightDir)) +
		g_MaterialAmbientColor * g_LightAmbientColor;      
    Output.Diffuse.a = 1.0f; 
	
    Output.TextureUV = input.tex;
    
    return Output;
}

VS_OUTPUT RenderInstancedNoNormalsVS(VSInstNoNormalsIn input)
{
    VS_OUTPUT Output = (VS_OUTPUT) 0;
    
    float4 InstancePosition = mul(float4(input.vPos, 1), input.mTransform);
    Output.Position = mul(InstancePosition, g_mWorldViewProjection);
     
 	Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * 
		+ g_MaterialAmbientColor;// * g_LightAmbientColor;      
    Output.Diffuse.a = 1.0f; 
	
    Output.TextureUV = input.tex;
    
    return Output;
}

technique11 RenderInstancedNoTex
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, RenderInstancedVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, SimplePS()));
    }  
}

technique11 RenderInstanced
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, RenderInstancedVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, RenderScenePS()));
    }  
}

technique11 RenderInstancedNoNormals
{
    pass p0
    {
        SetVertexShader(CompileShader(vs_4_0, RenderInstancedNoNormalsVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, RenderScenePS()));
        
        //SetRasterizerState(   NoCull );
    }  
}


