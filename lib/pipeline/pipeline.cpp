#include "pipeline.h"
#include "lib/render/data_points.h"

Pipeline::Pipeline(Context &ctx, PipelineInfo info)
{
  // TODO switch to pbr
  //SDL_GPUVertexAttribute vertexAttributes[7] = {};
  SDL_GPUVertexAttribute vertexAttributes[1] = {};
  SDL_GPUVertexBufferDescription vertexBufferDesc = {};

  if (info.has_vertex_input) {
    // submit vertex buffer to pipeline
    vertexAttributes[0] = {
      0,
      0,
      SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
      0
    };
    /*
    vertexAttributes[0] = {0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(PBRVertex, pos)};
    vertexAttributes[1] = {1, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(PBRVertex, normal)};
    vertexAttributes[2] = {2, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(PBRVertex, uv0)};
    vertexAttributes[3] = {3, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(PBRVertex, uv1)};
    vertexAttributes[4] = {4, 0, SDL_GPU_VERTEXELEMENTFORMAT_UINT4, offsetof(PBRVertex, joint0)};
    vertexAttributes[5] = {5, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, offsetof(PBRVertex, weight0)};
    vertexAttributes[6] = {6, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, offsetof(PBRVertex, color0)};

    vertexBufferDesc.slot = 0;
    vertexBufferDesc.pitch = sizeof(PBRVertex);
    vertexBufferDesc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
    */
    vertexBufferDesc.slot = 0;
    vertexBufferDesc.pitch = sizeof(float) * 3;
    vertexBufferDesc.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
  }

  // setup pipeline
  SDL_GPUColorTargetDescription desc_info = {
    .format = SDL_GetGPUSwapchainTextureFormat(ctx.gpu, ctx.window),
    .blend_state = {
      .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
      .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
      .color_blend_op = SDL_GPU_BLENDOP_ADD,
      .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
      .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
      .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
      .enable_blend = true,
    },
  };

  SDL_GPUGraphicsPipelineCreateInfo pipeline_create_info = {
    .vertex_shader = shader_load(ctx.gpu, {
      info.vertex_path, 
      SDL_GPU_SHADERSTAGE_VERTEX, 
      info.num_vert_uniforms, 
      0, 
      info.num_vert_storage_buffers
    }),
    .fragment_shader = shader_load(ctx.gpu, {
      info.fragment_path, 
      SDL_GPU_SHADERSTAGE_FRAGMENT, 
      info.num_frag_uniforms, 
      info.num_frag_samplers,
      info.num_frag_storage_buffers
    }),
    .vertex_input_state = {
      .vertex_buffer_descriptions = info.has_vertex_input ? &vertexBufferDesc : nullptr,
      .num_vertex_buffers = info.has_vertex_input ? 1u : 0u,
      .vertex_attributes = info.has_vertex_input ? vertexAttributes : nullptr,
      // TODO switch to pbr
      //.num_vertex_attributes = info.has_vertex_input ? 7u : 0u,
      .num_vertex_attributes = info.has_vertex_input ? 1u : 0u,
    },
    .primitive_type = info.primitive_type,
    .rasterizer_state = {
      .cull_mode = SDL_GPU_CULLMODE_BACK,
    },
    .depth_stencil_state = {
      .compare_op = SDL_GPU_COMPAREOP_LESS,
      .enable_depth_test = info.enable_depth_test,
      .enable_depth_write = info.enable_depth_write,
    },
    .target_info = {
      .color_target_descriptions = &desc_info,
      .num_color_targets = 1,
      .depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
      .has_depth_stencil_target = true,
    },
  };

  pipeline = SDL_CreateGPUGraphicsPipeline(ctx.gpu, &pipeline_create_info);
  if (!pipeline)
    SDL_Log("Failed to create compute pipeline: %s\n", SDL_GetError());
}

void Pipeline::start(SDL_GPURenderPass *p)
{
  pass = p;
  SDL_BindGPUGraphicsPipeline(pass, pipeline);
}