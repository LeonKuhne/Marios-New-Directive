cbuffer BufferData : register(b1, space1) { 
  uint chars[8];
};

struct PSInput {
  [[vk::location(0)]] float2 TexCoord : TEXCOORD0;
  [[vk::location(1)]] float3 WorldPos : TEXCOORD1;
  uint primitive_id : SV_PrimitiveID;
};

float4 main(PSInput input) : SV_TARGET
{
  float3 finalColor = float3(1.0, 1.0, 1.0);
  return float4(finalColor, 1.0);
}