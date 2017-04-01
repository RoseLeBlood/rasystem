//--------------------------------------------------------------------------------------
// File: ParticleEffect.fx
//
// Partikeleffekt
//
// 
//--------------------------------------------------------------------------------------

#include "MyEngine.fxh"
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

DepthStencilState DisableDepthWrite
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};
//--------------------------------------------------------------------------------------
// Globale Variablen und Strukturen
//--------------------------------------------------------------------------------------
cbuffer cbParticle
{
    float3 g_positions[4] =
    {
        float3( -1, 1, 0 ),
        float3( 1, 1, 0 ),
        float3( -1, -1, 0 ),
        float3( 1, -1, 0 ),
    };
    float2 g_texcoords[4] = 
    { 
        float2(0,1), 
        float2(1,1),
        float2(0,0),
        float2(1,0),
    };
    float g_fParticleRadius = 0.10;
    float g_fParticleDrawRadius = 0.025;
};

struct VS_OUTPUT_PARTICLE
{
    float4 Position   : SV_Position;   // vertex position 
    float4 Diffuse    : COLOR0;        // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float  Radius     : RADIUS;
};

//--------------------------------------------------------------------------------------
// Vertexshader für Partikeleffekt
//--------------------------------------------------------------------------------------
VS_OUTPUT_PARTICLE RenderParticleVS( float4 vPos  : POSITION, 
                                     float3 color : COLOR,
                                     float radius : RADIUS)
{
    VS_OUTPUT_PARTICLE Output;
    
    Output.Position = vPos;
    
    Output.Diffuse.rgb = color;   
    Output.Diffuse.a = 1.0f; 
    
    Output.Radius = radius; 
    
    return Output;    
}

//--------------------------------------------------------------------------------------
// Geometrieshader für Partikeleffekt
//--------------------------------------------------------------------------------------
[maxvertexcount(4)]
void RenderParticleGS(point VS_OUTPUT_PARTICLE input[1], inout TriangleStream<VS_OUTPUT> SpriteStream)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    
    for(int i=0; i<4; i++)
    {
        
		float3 position = g_positions[i] * input[0].Radius;	
        output.Position = mul(float4(position,1.0), g_mProj) +
		    mul(input[0].Position, g_mWorldViewProjection );
		
         //Alternative: 
        //float3 position = g_positions[i] * input[0].Radius;
        //position = mul( position, (float3x3)g_mInvView ) + input[0].Position;
        //output.Position = mul( float4(position,1.0), g_mWorldViewProjection );
        
        output.Diffuse = input[0].Diffuse;
        output.TextureUV = g_texcoords[i];

        SpriteStream.Append(output);
    }
    SpriteStream.RestartStrip();
}

[maxvertexcount(4)]
void RenderParticleToDepthMapGS(point VS_OUTPUT_PARTICLE input[1], inout TriangleStream<VS_OUTPUT_ToLightMap> SpriteStream)
{
    VS_OUTPUT_ToLightMap output = (VS_OUTPUT_ToLightMap)0;
    
    for(int i=0; i<4; i++)
    {
        float3 position = g_positions[i] * input[0].Radius;
        position = mul( position, (float3x3)g_mInvView ) + input[0].Position;
        output.Position = mul( float4(position,1.0), g_mWorldViewProjection );
        //Nur z wird gebraucht - Skalierung in y speichern
        output.vertexPosition.x = output.Position.z; 
        output.vertexPosition.y = output.Position.w;         

        SpriteStream.Append(output);
    }
    SpriteStream.RestartStrip();
}

[maxvertexcount(4)]
void RenderParticleToLightMapGS(point VS_OUTPUT_PARTICLE input[1], inout TriangleStream<VS_OUTPUT_ToLightMap> SpriteStream)
{
    VS_OUTPUT_ToLightMap output = (VS_OUTPUT_ToLightMap)0;
    
    for(int i=0; i<4; i++)
    {
        float3 position = g_positions[i] * input[0].Radius;
        position = mul( position, (float3x3)g_mInvView ) + input[0].Position;
        output.Position = mul( float4(position,1.0), g_mWorldViewProjection );
        //Nur z wird gebraucht - Skalierung in y speichern
        output.vertexPosition.x = output.Position.z; 
        output.vertexPosition.y = output.Position.w;         

        SpriteStream.Append(output);
    }
    SpriteStream.RestartStrip();
}
//--------------------------------------------------------------------------------------
// techniques
//--------------------------------------------------------------------------------------
technique11 RenderParticle
{
    pass P0
    {   
		SetVertexShader( CompileShader(vs_4_0, RenderParticleVS() ) );
        SetGeometryShader(CompileShader(gs_4_0, RenderParticleGS() ) );
        SetPixelShader( CompileShader(ps_4_0, RenderScenePS() ) );
        
        //SetBlendState( AdditiveBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
        //SetDepthStencilState( DisableDepthWrite, 0 );
    }
}

//-----------------------------------------------------------------------------
// Lightmapping
//-----------------------------------------------------------------------------
technique11 RenderToLightMap
{
    pass P0
    { 
		SetVertexShader(CompileShader(vs_4_0, RenderParticleVS()));  
        SetGeometryShader(CompileShader(gs_4_0, RenderParticleToLightMapGS() ) );
		SetPixelShader(CompileShader(ps_4_0, RenderToLightMapPS()));        
    }
}