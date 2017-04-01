//--------------------------------------------------------------------------------------
// Datei: Spotlicht.fx
// 
// Scheinwerfer (Spotlicht) mit Pixelshader
// 
// 
//--------------------------------------------------------------------------------------

#include "MyEngine.fxh" 

//--------------------------------------------------------------------------------------
// Vertexshader mit Spotlicht
//--------------------------------------------------------------------------------------
VS_OUTPUT_SPOT RenderMitSpotlichtVS( float4 vPos       : POSITION, 
                                     float3 vNormal    : NORMAL)
{
    VS_OUTPUT_SPOT Output = (VS_OUTPUT_SPOT)0;

    float4 vPosWorldSpace = mul(vPos, g_mWorld);
    //float4 vPosViewSpace  = mul(vPosWorldSpace, g_mView); //Position im Koordinatensystem der Kamera 
    float4 vPosViewSpace  = mul(vPos, g_mWorldView);        //Position im Koordinatensystem der Kamera 

    float3 vNormalWorldSpace = mul(vNormal, (float3x3)g_mWorld); // normal (world space)
    //float3 vNormalViewSpace  = mul(vNormal, (float3x3)g_mView); // normal (world space)
    float3 vNormalViewSpace  = mul(vNormal, (float3x3)g_mWorldView); // normal (world space)

    //Output.Position = mul(vPosViewSpace, g_mProj);
    Output.Position = mul(vPos, g_mWorldViewProjection);
    Output.PositionViewSpace = vPosViewSpace; 
    Output.NormalViewSpace = vNormalViewSpace; 
    return Output;    
}

//--------------------------------------------------------------------------------------
// Pixelshader Spotlicht
// Ohne Glanzlicht
// Ohne Dämpfung
// Berechnungen im ViewSpace (cosAlpha, NDotL)
// spotEffect mit smoothstep berechnet (daher Variable falloff nicht gebraucht)
//--------------------------------------------------------------------------------------
float4 RenderMitSpotlichtPS(VS_OUTPUT_SPOT In) : SV_Target
{ 
    float3 NViewSpace = normalize(In.NormalViewSpace); 
    float3 LViewSpace = 
        normalize(g_LightPosViewSpace.xyz - In.PositionViewSpace.xyz);
    
    float cosAlpha = dot(LViewSpace, -g_LightDirViewSpace);
    float spotEffect = smoothstep(g_cosHalfPhi, g_cosHalfTheta, cosAlpha); 
         
    float4 color; 
    color.rgb = spotEffect * 
        g_MaterialDiffuseColor * g_LightDiffuse  
        * max(0, dot(NViewSpace, LViewSpace))
        + g_MaterialAmbientColor; 
    color.a = 1.0f; 
    return color;
}

//--------------------------------------------------------------------------------------
// Rendern mit Spotlicht ohne Textur
//--------------------------------------------------------------------------------------
technique11 RenderMitSpotlicht
{
    pass P0
    {       
        SetVertexShader(CompileShader(vs_4_0, RenderMitSpotlichtVS()));
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0, RenderMitSpotlichtPS()));
    }
}
