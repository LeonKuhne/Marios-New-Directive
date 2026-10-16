#pragma once

#include <SDL3/SDL_gpu.h>
#include <vector>

class GPUTransferable {

private:
  inline static std::vector<GPUTransferable*> pending_updates;

protected:
  SDL_GPUDevice *gpu;

public:
  GPUTransferable(SDL_GPUDevice *gpu) : gpu(gpu)
  {
    markDirty();
  }

  void markDirty() 
  {
    pending_updates.push_back(this);
  }

  virtual void upload(SDL_GPUCopyPass *pass) = 0;

  static void processPendingUpdates(SDL_GPUCopyPass *pass)
  {
    for (GPUTransferable* obj : pending_updates)
      obj->upload(pass);
    pending_updates.clear();
  }
};