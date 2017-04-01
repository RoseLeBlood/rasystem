//--------------------------------------------------------------------------------------
// File: Postprocess.fx
//
// Postprocessing-Effekte (Blur, Glow, Depth-Of-Field)
//
// 
//--------------------------------------------------------------------------------------

#include "MyEngine.fxh"

Texture2D   g_depthTexture;            

cbuffer BlurParameters
{
	float3  g_BlurSigma = float3(1.5f, 1.5f, 1.5f); 
    float2	buffer_texel_size = float2(1.0f/256, 1.0f/256); //1.0f /width, 1.0f /height
};

cbuffer DoFParameters
{
    float d0 = 0.5f;
};

static const int blur_search_width = 8;
static const int blur_search_start = -blur_search_width;
static const int blur_search_end = blur_search_width + 1;
static const float  blur_scale = 2.0f;
static const float pi = 3.14159265f;

float3 Gauss(float d, float3 sigma)
{
	float3 sigma_prime = sigma * sigma * 2;
	return	exp(- abs(d) / sigma_prime) / (pi * sigma_prime);
}

float4 BlurPS(VS_OUTPUT In, uniform bool horizontal, uniform float alpha) : SV_Target
{
	float4 sum =  float4(0,0,0, alpha); 

	if(horizontal)
	{
		for(int i = blur_search_start;  i < blur_search_end; ++i)
			sum.rgb += Gauss(i, g_BlurSigma) * 
			g_MeshTexture.Sample(g_samWrap, In.TextureUV + 
			buffer_texel_size * float2(0.5f + 2.0f * i,0.5f)).rgb;
	}
	else
	{
		for(int i = blur_search_start;  i < blur_search_end; ++i)
			sum.rgb += Gauss(i, g_BlurSigma) * g_MeshTexture.Sample(g_samWrap, 
			In.TextureUV + buffer_texel_size * float2(0.5f, 0.5f + 2 * i)).rgb;
	}
    
    sum.rgb *= blur_scale;
	return sum;
}

technique11 RenderWithTextureFullScreen
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWithTextureFullScreenVS() ) );
        SetGeometryShader( NULL );
        SetPixelShader(CompileShader(ps_4_0, RenderSceneFullBrightPS() ) );
         
        //SetDepthStencilState(EnableDepthWrite, 0);
    }
}

technique11 BlurHorizontal
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWithTextureVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, BlurPS(true, 1.0f)));
         
        //SetDepthStencilState(EnableDepthWrite, 0);
    }
}

technique11 BlurVertical
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWithTextureVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, BlurPS(false, 0.5f)));
         
        //SetDepthStencilState(EnableDepthWrite, 0 );
    }
}

technique11 BlurHorizontalFullScreen
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWithTextureFullScreenVS()));
        SetGeometryShader( NULL );
        SetPixelShader(CompileShader(ps_4_0, BlurPS(true, 1.0f)));
         
        //SetDepthStencilState(EnableDepthWrite, 0);
    }
}

technique11 BlurVerticalFullScreen
{
    pass P0
    {       
        SetVertexShader(CompileShader(vs_4_0, RenderWithTextureFullScreenVS()));
        SetGeometryShader(NULL );
        SetPixelShader(CompileShader(ps_4_0, BlurPS(false, 0.5f)));
         
        //SetDepthStencilState(EnableDepthWrite, 0);
    }
}

float4 RenderWithDepthMapPS(VS_OUTPUT IN) : SV_Target
{
    float depth = 6 * 
        abs (g_depthTexture.Sample(g_samWrap, IN.TextureUV).x - d0) / (1 - d0);
    float level = clamp(depth, 0, 6);  
    return g_MeshTexture.SampleLevel(g_samWrap, IN.TextureUV, level);
}

technique11 RenderWithDoF
{
    pass P0
    {       
		SetVertexShader( CompileShader(vs_4_0, RenderWithTextureVS()));
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader(ps_4_0, RenderWithDepthMapPS()));
    }
}

technique11 RenderWithDoFFullScreen
{
    pass P0
    {       
		SetVertexShader(CompileShader(vs_4_0, RenderWithTextureFullScreenVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, RenderWithDepthMapPS()));
    }
}