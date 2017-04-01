//--------------------------------------------------------------------------------------
// File: Cellular.fx
//
// Zelluläre Textur
//
// 
//--------------------------------------------------------------------------------------
#include "MyEngine.fxh"

float3 g_FeaturePoints[16];
float  g_Coefficients[4]; 

//--------------------------------------------------------------------------------------
// Abstandsfunktionen
//--------------------------------------------------------------------------------------

float F(float3 pos)
{
    float maxdist = 0.0f;

    //Sortierte Abstände und zugehörige Nummern der Featurepoints
    //Maximal 4 - höhere Ornungen bringen nichts Neues und überfordern den Shader
    float dist[4];
    //int   nr[4];
    
    for(int i = 0; i < 4; i++)
    {
        dist[i] = 100;//1.4f; 
        //nr[i] = -1; 
    }    
    
    int sortiert = 0; 
    
    for(int i = 0; i < 16; i++)
    {
        float d = distance(pos, g_FeaturePoints[i]); 

        if(d < dist[3]) 
        {
            //alle mit größerem Abstand eins nach hinten rücken in der Liste,um Platz freizumachen        
            int n;
            for(n = 2; n >= 0; n--) //for(n = sortiert; d < dist[n]; n--) 
            {
                if(d < dist[n])  
                {  
                    dist[n + 1] = dist[n];
                    //nr[n + 1] = nr[n];
                }
                else break;
            }
            
            //an Position n einordnen
            dist[n + 1] = d;
            //nr[n + 1] = i;
            sortiert += 1;
        }
        if(d > maxdist)
        {
            maxdist = d;
        }
    }
    
    float summe = 0; 
    for(int i = 0; i < 4; i++)
        summe += g_Coefficients[i] * dist[i];
    //return (summe / maxdist);
    return 0.25f * summe;
}

//--------------------------------------------------------------------------------------
// Vertexshader
//--------------------------------------------------------------------------------------
VS_OUTPUT CellularVS(float4 vPos : POSITION,
                     float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output = (VS_OUTPUT)0;
    
    Output.Position = mul(vPos, g_mWorldViewProjection);
    
    Output.Diffuse = (float4) 1;   
    Output.TextureUV = vTexCoord0;//float2(0.5f*(vPos.x + 1), 0.5f*(vPos.y + 1));   
    
    return Output;    
}

//--------------------------------------------------------------------------------------
// Pixelshader
//--------------------------------------------------------------------------------------
float4 CellularPS(VS_OUTPUT In) : SV_Target
{ 
    float f = F(float3(In.TextureUV.x, In.TextureUV.y, 0));
    return In.Diffuse * (float4) f; 
}

technique11 Cellular
{
    pass P0
    {       
		SetVertexShader( CompileShader(vs_4_0, CellularVS()));
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, CellularPS()));
    }
}
