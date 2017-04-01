//--------------------------------------------------------------------------------------
// File: Noise.fxh
//
// Perlin-Noise und Fractional Brownian Motion
//
// 
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Perlin
//--------------------------------------------------------------------------------------
const float2 g_grads[8] = 
{
    float2(-1.0f, -1.0f),
    float2(-1.0f,  0.0f),
    float2(-1.0f,  1.0f),
    float2( 0.0f, -1.0f),
    float2( 0.0f,  1.0f),
    float2( 1.0f, -1.0f),
    float2( 1.0f,  0.0f),
    float2( 1.0f,  1.0f)
};

int g_indices[16][16]; 
   
cbuffer cbNoise
{
    int g_freq = 4; //32
    float g_bias = 0.5f;//0.4f;
    float g_scale = 0.5f;
    
    //float g_d = 0.025f; //Wird aus Frequenz berechnet (FindFbmNormal)
};    

Texture1D g_txColorsFromHeight;	     

//--------------------------------------------------------------------------------------
// Procedural
//--------------------------------------------------------------------------------------
float f(float t)
{
    return t * t * t * (t * (6 * t - 15) + 10); 
}

float h(float x, float z)
{
    uint i0 = floor(x); 
    uint i1 = i0 + 1; 

    uint j0 = floor(z); 
    uint j1 = j0 + 1; 

    float2 g00 = g_grads[g_indices[i0 % 16][j0 % 16]];
    float2 g01 = g_grads[g_indices[i0 % 16][j1 % 16]];
    float2 g10 = g_grads[g_indices[i1 % 16][j0 % 16]];
    float2 g11 = g_grads[g_indices[i1 % 16][j1 % 16]];

    float n00 = (x - i0) * g00.x + (z - j0) * g00.y;
    float n10 = (x - i1) * g10.x + (z - j0) * g10.y;
    
    float n01 = (x - i0) * g01.x + (z - j1) * g01.y;
    float n11 = (x - i1) * g11.x + (z - j1) * g11.y;

    float nx0 = n10 * f(x - i0) + n00 * (1-f((x - i0)));
    float nx1 = n11 * f(x - i0) + n01 * (1-f((x - i0)));

    return nx1 * f(z - j0) + nx0 * (1-f(z - j0)); 
}

float fbm(float x, float y)
{
    float s = 0; 
  
    //Maximum für c durch g_freq bestimmt 
    for(int c = 1; c <= g_freq; c *= 2)
    //for(int c = 1; g_freq / c >= 16; c *= 2) //Für kachelfähige Texturen müssen Frequenzen ein Vielfaches von 16 (Gittergrösse) sein
    {
        float dh = h(x * g_freq / c, y * g_freq / c);
        s = dh + 0.5f * s;
    } 
    return s; 
}

float4 FindFbmNormal(float2 vTexCoord0)
{
    float d =  1 / (2.0f * g_freq); 
    float s0 = fbm(vTexCoord0.x, vTexCoord0.y); 
    float s1 = fbm(vTexCoord0.x + d, vTexCoord0.y);  
    float s2 = fbm(vTexCoord0.x, vTexCoord0.y + d); 

    return normalize(float4(s0 - s1, s0 - s2, d, 0)); 
}