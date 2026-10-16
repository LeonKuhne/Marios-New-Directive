cbuffer SharedData : register(b0, space1) { 
  uint2 window_size;
};

cbuffer VertexData : register(b1, space1) { 
  uint2 offset : packoffset(c0.x);
  uint2 scale  : packoffset(c0.z);
};

struct VSOutput {
  float4 position : SV_Position;
  float2 texcoord : TEXCOORD0;
};

static const float2 positions[6] = {
  float2(-1.0, -1.0), // bl
  float2( 1.0, -1.0), // br
  float2(-1.0,  1.0), // tl
  float2( 1.0, -1.0), // br
  float2( 1.0,  1.0), // tr
  float2(-1.0,  1.0)  // tl
};

static const float2 texcoords[6] = {
  float2(0.0, 1.0), // bl
  float2(1.0, 1.0), // br
  float2(0.0, 0.0), // tl
  float2(1.0, 1.0), // br
  float2(1.0, 0.0), // tr
  float2(0.0, 0.0)  // tl
};

VSOutput main(uint vertexID : SV_VertexID) {
  VSOutput output;
  float2 uv_scale = (float2(scale) / float2(window_size)) * 2.0;
  float2 uv_offset = float2(offset) / float2(window_size) * 2.0 + uv_scale - 1.0;
  float2 pos = uv_offset + positions[vertexID] * uv_scale;
  output.position = float4(pos, 0.0, 1.0);
  output.texcoord = texcoords[vertexID];
  return output;
}