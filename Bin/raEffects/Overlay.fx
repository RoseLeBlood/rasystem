//--------------------------------------------------------------------------------------
// File: Overlay.fx
//
// Textur-Overlay (Minimap)
//
// 
//--------------------------------------------------------------------------------------
#include "MyEngine.fxh"

//--------------------------------------------------------------------------------------
//Render State
//--------------------------------------------------------------------------------------
BlendState AlphaBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ONE;//Destination Alpha soll erhalten bleiben
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

BlendState AdditiveBlending
{
    AlphaToCoverageEnable = FALSE;
    BlendEnable[0] = TRUE;
    SrcBlend = ONE;
    DestBlend = ONE;
    //SrcBlend = SRC_ALPHA;
    //DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ONE;//Destination Alpha soll erhalten bleiben
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

//--------------------------------------------------------------------------------------
// Vertexshader mit Textur, Overlay
//--------------------------------------------------------------------------------------
shared VS_OUTPUT RenderWithTextureOverlayVS(uint VertexId: SV_VertexID)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
	Output.Position = float4(QuadPositionTexCoords[VertexId].xy, 0, 1);
    
    //Beim Rendern mit Full-Bright-Pixelshader wird die Diffuse-Farbe nicht verwendet!
    //Output.Diffuse = g_MaterialDiffuseColor * g_LightDiffuse + 
    //    g_MaterialAmbientColor;   
    //Output.Diffuse.a = 1.0f; 
    
	Output.TextureUV = QuadPositionTexCoords[VertexId].zw;
    
    return Output;
}

//--------------------------------------------------------------------------------------
// Player Icon rendern
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderPlayerIconVS(uint VertexId: SV_VertexID)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
    //ToDO: dynamisch anpassen
    float foVW = 60; 
    float aspect = 640.0f / 480.0f; 
    float foVH = foVW / aspect;  
    
    //Die folgenden Werte könnten auch vorab berechnet werden
    float2 mapCenter    = 0.5f * (
        QuadPositionTexCoords[1] + QuadPositionTexCoords[2]).xy;
    float2 mapSizeHalf  = 0.5f * (
        QuadPositionTexCoords[1] - QuadPositionTexCoords[2]).xy;
    
    float2 relPos = 2 * float2(g_cameraPos.z / foVW, g_cameraPos.x / foVH); //(-1, -1) bis (1, 1) 

    Output.Position = 
        float4(relPos.x * mapSizeHalf.x, relPos.y * mapSizeHalf.y, 0, 0) + 
        float4(mapCenter, 0, 0) + 
        float4(0.02f * QuadPositionTexCoordsFullScreen[VertexId].xy, 0, 1);
    
    //Diffuse wird nicht ausgewertet
    //Output.Diffuse = g_MaterialDiffuseColor * g_LightDiffuse + 
    //    g_MaterialAmbientColor;   
    //Output.Diffuse.a = 1.0f; 
    
	Output.TextureUV = QuadPositionTexCoordsFullScreen[VertexId].zw;
    
    return Output;
}

float4 RenderPlayerIconPS(VS_OUTPUT In) : SV_Target
{ 
    return g_PlayerIconTexture.Sample(g_samWrap, In.TextureUV);
}

technique11 RenderWithTextureOverlay
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWithTextureOverlayVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, RenderSceneFullBrightPS()));
        //SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
    }
    
    //An g_cameraPos ein Miniquad einfügen
    pass P1
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderPlayerIconVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, RenderPlayerIconPS()));
        //SetBlendState(AlphaBlending, float4(0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF);
    }
}