//--------------------------------------------------------------------------------------
// File: TesselationEffect.fx
//
// Rekursives Erzeugen von Geometrie
//
// 
//--------------------------------------------------------------------------------------

#include "MyEngine.fxh"
DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ALL;
};

DepthStencilState DisableDepthWrite
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

float4 g_testcampos = float4(-1, 0, 1, 1); 
float  g_split_ratio = 10;

struct POSITIONTEXTURED
{
    float4 Position   : POSITION;      
    float2 TextureUV  : TEXCOORD0;     
};

POSITIONTEXTURED PassThroughVS(POSITIONTEXTURED input)
{
	return input;
}

[maxvertexcount(6)]
void CreateGeometryGS(triangle POSITIONTEXTURED input[3], 
    inout TriangleStream<POSITIONTEXTURED> Stream)
{
    bool bSplitTexture = false;  
    
    float4 p0 = 0.5f * (input[1].Position + input[2].Position);
	float4 p1 = 0.5f * (input[0].Position + input[2].Position);
	float4 p2 = 0.5f * (input[0].Position + input[1].Position);
	
    float4 v0 = input[2].Position - input[1].Position;
	float4 v1 = input[2].Position - input[0].Position;
	float4 v2 = input[1].Position - input[0].Position;
	
    float d0= length(p0 - g_testcampos); 
    float d1= length(p1 - g_testcampos); 
    float d2= length(p2 - g_testcampos); 

	float r0 = d0 / length(v0);  
	float r1 = d1 / length(v1);  
	float r2 = d2 / length(v2);  
	
	//alle drei Seiten betrachten, damit Nachbarn immer zusammen splitten
	if((r0 >= g_split_ratio) && (r1 >= g_split_ratio) && (r2 >= g_split_ratio)) 
	{
	    POSITIONTEXTURED output = (POSITIONTEXTURED)0;
	    for( int v = 0; v < 3; v++ )
        {
            Stream.Append(input[v]);
        }
		Stream.RestartStrip();
	}
	else //Dreieck in zwei aufteilen
	{
        float4 pos[6];
        pos[0] = p0;
        pos[1] = input[0].Position; 
        pos[2] = input[1].Position; 
        pos[3] = p0; 
        pos[4] = input[2].Position; 
        pos[5] = input[0].Position; 
        
        float2 tex[6];
        tex[0] = 0.5f * (input[1].TextureUV + input[2].TextureUV);
        tex[1] = input[0].TextureUV;
        tex[2] = input[1].TextureUV;
        tex[3] = tex[0];  
        tex[4] = input[2].TextureUV; 
        tex[5] = input[0].TextureUV;

        POSITIONTEXTURED output = (POSITIONTEXTURED)0;
        
        for(uint v = 0; v < 6; v++ )
        {
            output.Position = pos[v]; 

            if(bSplitTexture)
                output.TextureUV = input[v % 3].TextureUV;
            else
                output.TextureUV = tex[v]; 
                
            Stream.Append(output);
            
            if(v % 3 == 2) Stream.RestartStrip();
        }
    }
}

GeometryShader gsStreamOut = ConstructGSWithSO( 
CompileShader(gs_4_0, CreateGeometryGS()), 
"POSITION.xyz; TEXCOORD0.xy");

technique11 CreateGeometry
{
    pass p0
    {
        SetDepthStencilState(DisableDepth, 0 );
        SetDepthStencilState(DisableDepthWrite, 0 );
        
        SetVertexShader(CompileShader(vs_4_0, PassThroughVS()));
        SetGeometryShader(gsStreamOut);
        SetPixelShader(NULL);
    }
};
