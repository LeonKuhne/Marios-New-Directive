#pragma once

#include "lib/collisions/collider_body.h"
#include "lib/shapes/gameobjects/solid.h"

class Body : public Solid 
{
public:
  struct Info
  {
    Solid::Info solid;
  };

  float density;
  ColliderBody *body;

  Body(const Info &info);
};