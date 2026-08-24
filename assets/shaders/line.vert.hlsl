cbuffer View : register(b0, space1) {
  column_major float4x4 view_projection;
};

cbuffer LineStart : register(b1, space1) {
  float3 start;
};

cbuffer LineEnd : register(b2, space1) {
  float3 end;
};

struct VSOutput {
  float4 position : SV_Position;
};

VSOutput main(uint vertexID : SV_VertexID)
{
  VSOutput output;
  float3 world = (vertexID == 0) ? start : end;
  output.position = mul(view_projection, float4(world, 1.0));
  return output;
}
