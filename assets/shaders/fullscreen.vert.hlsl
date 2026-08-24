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
  float2 pos = positions[vertexID];
  output.position = float4(pos, 0.0, 1.0);
  output.texcoord = texcoords[vertexID];
  return output;
}