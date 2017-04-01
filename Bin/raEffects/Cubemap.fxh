//--------------------------------------------------------------------------------------
// Datei: Cubmap.fxh
// 
// Rendern in Cubmap mittels Geometrieshader
// 
//--------------------------------------------------------------------------------------

// Stellt allen Effektdateien die Shader 
// für das Rendern in die Cubemap zur Verfügung.
// Entsprechende Techniques müssen aber in jeder Effektdatei definiert werden.

//--------------------------------------------------------------------------------------
// Vertexshader für Rendern in Cubemap mit voller Helligkeit
//--------------------------------------------------------------------------------------
shared VS_OUTPUT VS_CubeMap(float4 vPos : POSITION, 
                     float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output = (VS_OUTPUT)0.0f;
    Output.Position = mul(vPos, g_mWorld);
    
    //volle Helligkeit
    Output.Diffuse = (float4)1.0f; 

    Output.TextureUV = vTexCoord0;
    return Output;
}

//--------------------------------------------------------------------------------------
// Vertexshader für Rendern in Cubemap mit Richtungslicht
//--------------------------------------------------------------------------------------
shared VS_OUTPUT VS_CubeMapDirectional(float4 vPos : POSITION, 
                     float3 vNormal : NORMAL,
                     float2 vTexCoord0 : TEXCOORD0)
{
    VS_OUTPUT Output = (VS_OUTPUT)0.0f;
    Output.Position = mul(vPos, g_mWorld);
    
    //wie RenderDirectional
    float3 vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); 
    Output.Diffuse.rgb = g_MaterialDiffuseColor * g_LightDiffuse * 
                         max(0,dot(vNormalWorldSpace, g_vLightDir)) + 
                         g_MaterialAmbientColor;   
    Output.Diffuse.a = 1.0f; 

    Output.TextureUV = vTexCoord0;
    return Output;
}

//--------------------------------------------------------------------------------------
// Geometrieshader für Rendern in Cubemap
//--------------------------------------------------------------------------------------
[maxvertexcount(18)]
shared void GS_CubeMap( triangle VS_OUTPUT input[3], 
    inout TriangleStream<PS_CUBEMAP_IN> CubeMapStream )
{
    for( int f = 0; f < 6; ++f )
    {
        PS_CUBEMAP_IN Output = (PS_CUBEMAP_IN) 0;
        Output.RTIndex = f;  //0 = rechts, 1 = links, 2 = oben
                             //3 = unten, 4 = vorn, 5 = hinten 
        for( int v = 0; v < 3; v++ )
        {
            Output.Position  = mul(input[v].Position, g_mViewCM[f] );
            Output.Position  = mul(Output.Position, g_mProj );
            Output.Diffuse   = input[v].Diffuse;

            Output.TextureUV = input[v].TextureUV;
            CubeMapStream.Append(Output);
        }
        CubeMapStream.RestartStrip();
    }
}

//--------------------------------------------------------------------------------------
// Pixelshader für Rendern in Cubemap
//--------------------------------------------------------------------------------------
shared float4 PS_CubeMap(PS_CUBEMAP_IN In) : SV_Target
{
    return g_MeshTexture.Sample(g_samWrap, In.TextureUV) * In.Diffuse;
}
