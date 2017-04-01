//-----------------------------------------------------------------------------
// Datei: Beleuchtungstechniken.fx
//
// Beleuchtungstechniken 
// 
//-----------------------------------------------------------------------------
#include "MyEngine.fxh"
#include "Cubemap.fxh" 

float g_falloff = 1.0f; 

VS_OUTPUT RenderWithRedColorVS(float4 vPos : POSITION)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);
    Output.Diffuse = float4(1, 0, 0, 1); 
    
    return Output;    
}

float4 TilePS(VS_OUTPUT In) : SV_Target
{ 
    float2 f = 2 * floor(2 * In.TextureUV) - 1;
    float c = 0.5f * (f.x * f.y + 1); 
    
    return In.Diffuse * float4(c, c, c, 1);
}

//--------------------------------------------------------------------------------------
// Richtungslicht mit Texturkoordinaten
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderDirectionalVS( float4 vPos       : POSITION, 
                               float3 vNormal    : NORMAL,
                               float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);
    
    vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); 

    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * 
                         max(0,dot(vNormalWorldSpace, g_vLightDir)) + 
                         g_MaterialAmbientColor * g_LightAmbientColor;   
    Output.Diffuse.a = 1.0f; 
    
    Output.TextureUV = vTexCoord0; 

    return Output;    
}

//--------------------------------------------------------------------------------------
// Richtungslicht ohne Texturkoordinaten
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderDirectionalNoTexCoordsVS( float4 vPos       : POSITION, 
										  float3 vNormal    : NORMAL)
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);
    
    vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); 

    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * 
                         max(0,dot(vNormalWorldSpace, g_vLightDir)) + 
                         g_MaterialAmbientColor;   
    Output.Diffuse.a = 1.0f; 

    return Output;    
}

VS_OUTPUT RenderMitPunktlichtVS( float4 vPos       : POSITION, 
                                 float3 vNormal    : NORMAL,
                                 float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output;

    float4 worldPos  = mul(vPos,      g_mWorld);
    //float4 cameraPos = mul(worldPos,  g_mView); //Position im Koordinatensystem der Kamera 
    //Output.Position  = mul(cameraPos, g_mProj);
    float4 cameraPos = mul(vPos,      g_mWorldView); //Position im Koordinatensystem der Kamera 
    Output.Position  = mul(vPos,      g_mWorldViewProjection);

    float3 vNormalWorldSpace;
    vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); // normal (world space)

    float3 toLight = (g_LightPos - worldPos).xyz;
    float lightDist = length(toLight);
    float fAtten = 1.0/dot(g_LightAttenuation, 
        float4(1,lightDist,lightDist*lightDist,0));
        
    float3 lightDir = normalize(toLight); 
    
    float3 halfAngle = normalize(normalize(-cameraPos) + lightDir);
    float NDotH = max(0, dot(halfAngle, vNormalWorldSpace));

    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * 
		max(0,dot(vNormalWorldSpace, lightDir)) * fAtten 
		+ g_LightSpecular * pow(NDotH, g_SpecularPower) * fAtten   
        + g_MaterialAmbientColor; 
    Output.Diffuse.a = 1.0f; 
    
    Output.TextureUV = vTexCoord0; 
    
    return Output;    
}

//--------------------------------------------------------------------------------------
// Muss jede Effektdatei haben
//--------------------------------------------------------------------------------------
technique11 RenderToCubeMap
{
    pass p0
    {
        //SetVertexShader( CompileShader(vs_4_0, VS_CubeMap() ) );
        SetVertexShader( CompileShader(vs_4_0, VS_CubeMapDirectional() ) );
        SetGeometryShader( CompileShader(gs_4_0, GS_CubeMap() ) );
        SetPixelShader( CompileShader(ps_4_0, PS_CubeMap() ) );
        
        //SetBlendState( NoBlend, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }
};

//--------------------------------------------------------------------------------------
// Techniques 
//--------------------------------------------------------------------------------------

fxgroup Farbe
{
	technique11 RenderWithColor
	{
		pass P0
		{       
			SetVertexShader( CompileShader(vs_4_0, RenderWithRedColorVS()));
			SetGeometryShader( NULL );
			SetPixelShader( CompileShader(ps_4_0, SimplePS()));
		}
	}
}

fxgroup Textur
{
	technique11 RenderWithTexture
	{
		pass P0
		{       
			SetVertexShader( CompileShader(vs_4_0, RenderWithTextureVS() ) );
			SetGeometryShader( NULL );
			SetPixelShader( CompileShader(ps_4_0, RenderScenePS() ) );
		}
	}
}

fxgroup Richtungslicht
{
	technique11 RenderDirectionalNoTexCoords
	{
		pass P0
		{       
			SetVertexShader( CompileShader(vs_4_0, RenderDirectionalNoTexCoordsVS() ) );
			SetGeometryShader( NULL );
			SetPixelShader( CompileShader(ps_4_0, SimplePS() ) );
		}
	}
	
	technique11 RenderDirectional
	{
		pass P0
		{       
			SetVertexShader( CompileShader(vs_4_0,RenderDirectionalVS()));
			SetGeometryShader( NULL );
			SetPixelShader( CompileShader(ps_4_0, SimplePS()));
		}
	}
	
	technique11 RenderDirectionalTextured
	{
		pass P0
		{       
			SetVertexShader(CompileShader(vs_4_0,RenderDirectionalVS()));
			SetGeometryShader( NULL );
			SetPixelShader(CompileShader(ps_4_0, RenderScenePS()));
		}
	}	
}

fxgroup Punktlicht
{
	technique11 RenderMitPunktlicht
	{
		pass P0
		{       
			SetVertexShader( CompileShader(vs_4_0, RenderMitPunktlichtVS()));
			SetGeometryShader( NULL );
			SetPixelShader( CompileShader(ps_4_0, SimplePS()));
		}
	}
}

//Glanzlicht und Spotlicht fehlt noch Vertexsahder,  vgl. Kap08_01

