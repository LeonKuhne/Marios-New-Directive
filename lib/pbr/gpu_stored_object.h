#pragma once

#include <SDL3/SDL_gpu.h>
#include <vector>

class GPUStoredObject {

private:
  inline static std::vector<GPUStoredObject*> pending_updates;

protected:
  SDL_GPUDevice *gpu;

public:
  GPUStoredObject(SDL_GPUDevice *gpu) : gpu(gpu) {
    pending_updates.push_back(this);
  }
  virtual void upload(SDL_GPUCopyPass *pass) = 0;

  static void processPendingUpdates(SDL_GPUCopyPass *pass) {
    for (GPUStoredObject* obj : pending_updates) {
      obj->upload(pass);
    }
    pending_updates.clear();
  }
};