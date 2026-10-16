cbuffer SharedData : register(b0, space3) { 
  uint2 window_size;
};

cbuffer FragmentData: register(b1, space3) { 
  uint num_chars;
};

cbuffer PipelineData : register(b2, space3) { 
  uint2 atlas_glyph_size;
  uint atlas_width;
};

Texture2D atlas : register(t0, space2);
StructuredBuffer<uint> text : register(t1, space2);
SamplerState atlas_sampler : register(s0, space2);

struct PSInput {
  [[vk::location(0)]] float2 uv : TEXCOORD0;
  [[vk::location(1)]] uint index : TEXCOORD1;
  uint primitive_id : SV_PrimitiveID;
};

float4 main(PSInput input) : SV_TARGET
{
  uint char_idx = min((uint)(input.uv.x * float(num_chars)), num_chars - 1);
  uint atlas_idx = text[char_idx] - 32;

  float2 glyph_size = float2(atlas_glyph_size);
  float2 atlas_size = float2(atlas_width, glyph_size.y);

  float2 glyph_offset = float2(atlas_idx * glyph_size.x, 0.0);
  float2 glyph_uv = float2(frac(input.uv.x * num_chars), input.uv.y);
  float2 atlas_uv = (glyph_offset + glyph_uv * glyph_size) / atlas_size;

  return atlas.Sample(atlas_sampler, atlas_uv);
  //float3 finalColor = float3(1.0, 1.0, 1.0);
  //return float4(finalColor, 1.0);
}