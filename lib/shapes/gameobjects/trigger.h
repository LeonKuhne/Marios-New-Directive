#pragma once

#include "lib/shapes/gameobjects/solid.h"

class Trigger : public Solid 
{
public:
  struct Info
  {
    Solid::Info solid;
  };

  Trigger(const Info &info);
};