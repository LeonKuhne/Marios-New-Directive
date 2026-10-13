#pragma once

class Scene;

class Widget
{
public:
  virtual void render(Scene &scene) = 0;
};