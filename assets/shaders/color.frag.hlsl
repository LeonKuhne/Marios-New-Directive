cbuffer Color : register(b0, space3) {
  float3 color;
}; 

struct PSInput {
  [[vk::location(0)]] float2 TexCoord : TEXCOORD0;
  [[vk::location(1)]] float3 WorldPos : TEXCOORD1;
  uint primitive_id : SV_PrimitiveID;
};

float hash11(float p) {
  return frac(sin(p * 12.9898) * 43758.5453);
}

float4 main(PSInput input) : SV_TARGET
{
  // Tuning constants
  const float GRADIENT_MIX = 0.4;
  const float GRADIENT_MID_G = 0.25;

  const float TRI_SHADE_MIN = 0.97;
  const float TRI_SHADE_MAX = 1.03;
  const float TRI_SOFTNESS = 0.45;

  const float EDGE_BASE = 0.995;
  const float EDGE_AMPLITUDE = 0.005;
  const float EDGE_FREQ = 18.0;

  // Keep the original whole-mesh gradient across UV space.
  float3 gradientColor = float3(input.TexCoord.x, GRADIENT_MID_G, input.TexCoord.y);
  float3 baseColor = lerp(gradientColor, color, GRADIENT_MIX);

  // Per-triangle variation using primitive id makes adjacent triangles easier to distinguish.
  float triVariation = hash11((float)input.primitive_id + 1.0);
  float triShadeRaw = lerp(TRI_SHADE_MIN, TRI_SHADE_MAX, triVariation);
  float triShade = lerp(1.0, triShadeRaw, TRI_SOFTNESS);

  // Subtle edge-like modulation to avoid flat-looking surfaces.
  float edgeWave = 0.5 + 0.5 * sin((input.TexCoord.x + input.TexCoord.y) * EDGE_FREQ);
  float edgeMod = EDGE_BASE + EDGE_AMPLITUDE * edgeWave;

  float3 finalColor = baseColor * triShade * edgeMod;
  return float4(finalColor, 1.0);
}