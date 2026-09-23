#pragma once

#include "lib/collisions/collider_body.h"

class Player;

class PlayerBody : public ColliderBody
{
public:
  Player& player;

  PlayerBody(Player& player, uint8_t collider_group, btRigidBodyConstructionInfo& info) 
    : ColliderBody(collider_group, info), player(player) 
  {
    on_enter = [this](CollisionEvent& event) { this->onEnter(event); };
    on_exit = [this](CollisionEvent& event) { this->onExit(event); };
  }

private:
  bool isPlayerCollidingWithSolid(const ColliderBody *other);

  void onEnter(CollisionEvent& event);
  void onExit(CollisionEvent& event);
};