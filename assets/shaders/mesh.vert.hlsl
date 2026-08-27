cbuffer BufferData : register(b0, space1) { 
  column_major float4x4 projection       : packoffset(c0);
  column_major float4x4 model            : packoffset(c4);
  float3 center_offset                   : packoffset(c8);
  float3 scale                           : packoffset(c9);
};

struct VSOutput {
  float4 position : SV_Position;
  [[vk::location(0)]] float2 texcoord : TEXCOORD0;
  [[vk::location(1)]] float3 world_pos : TEXCOORD1;
};

struct VSInput {
  [[vk::location(0)]] float3 pos : POSITION;
};

VSOutput main(VSInput input) {
  VSOutput output;
  float3 localPos = input.pos - center_offset;
  float4 worldPos = mul(model, float4(localPos * scale, 1.0f));
  output.position = mul(projection, worldPos);
  output.texcoord = input.pos.xz * 0.5f + 0.5f; // Simple UV mapping based on world position
  output.world_pos = worldPos.xyz;
  return output;
}