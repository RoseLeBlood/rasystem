//--------------------------------------------------------------------------------------
// File: Reflection.fx
//
// Effekte für reflektierende Objekte
//
// 
//--------------------------------------------------------------------------------------

#include "MyEngine.fxh"

//-----------------------------------------------------------------------------
// Speziell für Cubemap.
//-----------------------------------------------------------------------------
TextureCube  g_cubeMapTexture;
float reflectivity = 0.5f;                     

float r = 0.9f;             //Verhältnis der Brechungsindizes (Ratio)
float transmittance = 0.9f;   

float3 r3 = {0.9f, 0.91f, 0.92f};
float fres0 = 0.003f; 
float fresPow = 5.0f;

//-----------------------------------------------------------------------------
// Textures.
//-----------------------------------------------------------------------------
SamplerState CubeTextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//-----------------------------------------------------------------------------
// Vertex Shaders.
//-----------------------------------------------------------------------------
void VS_Reflection(float4 inPosition    : POSITION,
                   float3 inNormal      : NORMAL,
                   out float4 position  : SV_Position,
                   out float3 EnvTex    : TEXCOORD0)
{
    float3 posW =(mul(inPosition, g_mWorld)).xyz;

    float3 N = normalize(mul(inNormal, (float3x3)g_mWorld));
    float3 I = posW - g_cameraPos;
	
    float3 R = I - 2 * dot(I, N) * N; 
	
	EnvTex = R;
	
    position = mul(inPosition, g_mWorldViewProjection);
}

void VS_Refraction(float4 inPosition   : POSITION,
                   float3 inNormal     : NORMAL,
                   out float4 position : SV_Position,
                   out float3 EnvTex   : TEXCOORD0)
{
    float3 posW =(mul( inPosition, g_mWorld )).xyz;

    float3 N = normalize(mul(inNormal, (float3x3)g_mWorld));    
    
    float3 I = normalize(posW - g_cameraPos);
    
    float NdotI = dot(I, N);
    float cosT2 = 1.0f - r * r * (1.0f - NdotI * NdotI);
    
    float3 T = r * I - 
    ((r * NdotI + sqrt(abs(cosT2))) * N);
    
    float3 vTrans = T * (float3)(cosT2 > 0);
    
    EnvTex = vTrans;
    position = mul(inPosition, g_mWorldViewProjection);
}

void VS_Dispersion(float4 inPosition         : POSITION,
                   float3 inNormal           : NORMAL,
                   out float4 position       : SV_Position,
                   out float reflectionFactor: COLOR,
                   out float3 R              : TEXCOORD0,
                   out float3 TRed           : TEXCOORD1,
                   out float3 TGreen         : TEXCOORD2,
                   out float3 TBlue          : TEXCOORD3)
{
    float3 posW =(mul( inPosition, g_mWorld )).xyz;

    float3 N = normalize(mul(inNormal, (float3x3)g_mWorld));    
    
    float3 I = normalize(posW - g_cameraPos);
    
    R  = reflect(I, N);
    
    reflectionFactor = fres0 + (1 - fres0) * pow(1 + dot(I, N), fresPow); 
    
    TRed    = refract(I, N, r3.x);
    TGreen  = refract(I, N, r3.y);
    TBlue   = refract(I, N, r3.z);
    
    position = mul(inPosition, g_mWorldViewProjection);
}
 
//-----------------------------------------------------------------------------
// Pixel Shaders.
//-----------------------------------------------------------------------------
float4 PS_Reflection(float4 Position : SV_Position, 
                     float3 Tex      : TEXCOORD0) : SV_Target
{
    return reflectivity * g_cubeMapTexture.Sample(CubeTextureSampler, Tex);
}

float4 PS_Refraction(float4 Position : SV_Position, 
                     float3 Tex : TEXCOORD0) : SV_Target
{
   float4 color = {1, 1, 1, 1};
   return color * transmittance * g_cubeMapTexture.Sample(CubeTextureSampler, Tex);
}

float4 PS_Dispersion(float4 Position        : SV_Position, 
                     float reflectionFactor : COLOR,
                     float3 R               : TEXCOORD0,
                     float3 TRed            : TEXCOORD1, 
                     float3 TGreen          : TEXCOORD2, 
                     float3 TBlue           : TEXCOORD3 ) : SV_Target
{
   float4 reflectedColor = g_cubeMapTexture.Sample(CubeTextureSampler, R);
   float4 refractedColor;
   refractedColor.r = g_cubeMapTexture.Sample(CubeTextureSampler, TRed).r;
   refractedColor.g = g_cubeMapTexture.Sample(CubeTextureSampler, TGreen).g;
   refractedColor.b = g_cubeMapTexture.Sample(CubeTextureSampler, TBlue).b;
   refractedColor.a = 1;
    
   return (1-reflectionFactor) * refractedColor + reflectionFactor * 
       reflectedColor;
   //return transmittance * refractedColor;
}
//-----------------------------------------------------------------------------
// Techniques
//-----------------------------------------------------------------------------
technique11 Reflection
{
    pass p0
    {
        SetVertexShader( CompileShader(vs_4_0, VS_Reflection() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, PS_Reflection() ) );
    }
}

technique11 Refraction
{
    pass p0
    {
        SetVertexShader( CompileShader(vs_4_0, VS_Refraction() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, PS_Refraction() ) );
    }
}

technique11 Dispersion
{
    pass p0
    {
        SetVertexShader( CompileShader(vs_4_0, VS_Dispersion() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, PS_Dispersion() ) );
    }
}


void VS_Reflection1(float4 inPosition   : POSITION,
                   float3 inNormal      : NORMAL,
                   out float4 position  : SV_Position,
                   out float3 EnvTex    : TEXCOORD0)
{
    float3 posW =(mul(inPosition, g_mWorld)).xyz;

    float3 N = normalize(mul(inNormal, (float3x3)g_mWorld));
    float3 I = posW - g_cameraPos;
	
    float3 R = I - 2 * dot(I, N) * N; 

	//Verschiebung berücksichtigen	
	float3 reflCameraPos = float3(0, -0.5f, 0); //Position der für das Rendern in die Cubemap 
                                                //verwendeten Kamera. Hier wie im Beispiel 
                                                //Kap14_07, besser auch als globale Variable übergeben		
    float3 D = posW - reflCameraPos; 
    EnvTex = dot(D, N) * R + dot(R, N) * posW;  
    
    position = mul(inPosition, g_mWorldViewProjection);
}

technique11 Reflection1
{
    pass p0
    {
        SetVertexShader( CompileShader(vs_4_0, VS_Reflection1() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, PS_Reflection() ) );
    }
}
