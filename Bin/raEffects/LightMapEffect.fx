//-----------------------------------------------------------------------------
// Datei: LightMapEffect.fx
//
// Lightmapping 
// 
//-----------------------------------------------------------------------------

#include "MyEngine.fxh"

//-----------------------------------------------------------------------------
// RenderToLightMap
//-----------------------------------------------------------------------------
technique11 RenderToLightMap
{
    pass P0
    { 
		SetVertexShader(CompileShader(vs_4_0, RenderToLightMapVS()));  
		SetGeometryShader(NULL); 
		SetPixelShader(CompileShader(ps_4_0, RenderToLightMapPS()));        
    }
}

//-----------------------------------------------------------------------------
// RenderWithLightMap
//-----------------------------------------------------------------------------
struct VS_OUTPUT_WithLightMap
{
    float4 Position		        : SV_Position;
    float2 decalTexCoord        : TEXCOORD0;   
    float4 shadowTexCoord		: TEXCOORD1;    
    float4 PositionViewSpace    : POSITIONT;     //Für SpotEffekt
    float3 NormalViewSpace      : NORMALT;  
};

VS_OUTPUT_WithLightMap RenderWithLightMapVS(
    float4 inPosition  : POSITION, 
    float3 inNormal    : NORMAL,
    float2 decalTexCoord : TEXCOORD0)
{
    VS_OUTPUT_WithLightMap Output = (VS_OUTPUT_WithLightMap)0;
    Output.Position = mul(inPosition, g_mWorldViewProjection);
    Output.decalTexCoord = decalTexCoord; 
    Output.shadowTexCoord = mul(inPosition, g_mLightViewProjection);

    //SpotEffekt    
    float4 vPosWorldSpace = mul(inPosition, g_mWorld);
    float4 vPosViewSpace  = mul(vPosWorldSpace, g_mView); //Position im Koordinatensystem der Kamera 
    float3 vNormalWorldSpace = mul(inNormal, (float3x3)g_mWorld); // normal (world space)
    float3 vNormalViewSpace  = mul(vNormalWorldSpace, (float3x3)g_mView); // normal (world space)
    Output.PositionViewSpace = vPosViewSpace; 
    Output.NormalViewSpace = vNormalViewSpace; 
    
    return Output;    
}

float4 RenderWithLightMapPS(VS_OUTPUT_WithLightMap IN) : SV_Target
{
    float2 projectedShadowTexCoord;
    projectedShadowTexCoord[0] =
         IN.shadowTexCoord.x/IN.shadowTexCoord.w/2.0f + 0.5f;
    projectedShadowTexCoord[1] =
         -IN.shadowTexCoord.y/IN.shadowTexCoord.w/2.0f + 0.5f;
        
    float depth = g_shadowTexture.Sample(g_samClamp, projectedShadowTexCoord).x;
    
    //float dist = IN.shadowTexCoord.z / IN.shadowTexCoord.w / 2.0f + 0.5f; 
    float dist = IN.shadowTexCoord.z / IN.shadowTexCoord.w;  //0 bis 1
    //float4 color = (depth >= dist - 0.04f)? (float4) 1.0f : (float4)0.5f; 
    float4 color = (depth >= dist - 0.1f)? (float4) 1.0f : (float4)0.1f; 
    
    //SpotEffekt  
    float3 NViewSpace = normalize(IN.NormalViewSpace); 
    float3 LViewSpace = 
        normalize(g_LightPosViewSpace.xyz - IN.PositionViewSpace);
    float cosAlpha = dot(LViewSpace, -g_LightDirViewSpace);
    float spotEffect = smoothstep(g_cosHalfPhi, g_cosHalfTheta, cosAlpha); 

    float4 retColor = spotEffect * max(0, dot(NViewSpace, LViewSpace)) * color; 
    return (retColor + g_LightAmbientColor) * 
        g_MeshTexture.Sample(g_samWrap, IN.decalTexCoord);
}

technique11 RenderWithLightMap
{
    pass P0
    { 
		SetVertexShader(CompileShader(vs_4_0, RenderWithLightMapVS())); 
		SetGeometryShader(NULL);  
		SetPixelShader(CompileShader(ps_4_0, RenderWithLightMapPS()));        
    }
}

