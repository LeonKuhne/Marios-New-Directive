#pragma once

#include <algorithm>
#include <functional>
#include <vector>
#include <SDL3/SDL.h>

class Scene;
class Knife;

enum KeyState : uint8_t
{
  UP,
  DOWN,
  HELD,
};

class Keyboard
{
  std::vector<std::pair<size_t, std::function<void()>>> bindings;
  std::vector<SDL_Keycode> held_keys;

public:
  void setup(Scene &scene);

  void toggle(SDL_Keycode key, KeyState state)
  {
    switch (state)
    {
    case DOWN:
    {
      if (isHeld(key))
        return;
      runKey(key, state);
      setHeld(key, true);
      break;
    }
    case UP:
    {
      runKey(key, state);
      setHeld(key, false);
      break;
    }
    case HELD:
    {
      runKey(key, state);
      break;
    }
    }
  }

  void process_held() 
  {
    for (SDL_Keycode key : held_keys)
      runKey(key, HELD);
  }

private:
  size_t makeHash(SDL_Keycode key, KeyState state) const
  {
    const size_t state_hash = std::hash<int>{}(static_cast<int>(state));
    const size_t key_hash = std::hash<SDL_Keycode>{}(key);
    return state_hash ^ (key_hash << 1);
  }

  void bindKey(SDL_Keycode key, KeyState state, const std::function<void()>& action)
  {
    const size_t hash = makeHash(key, state);
    for (auto &binding : bindings)
    {
      if (binding.first == hash)
      {
        binding.second = action;
        return;
      }
    }
    bindings.emplace_back(hash, action);
  }

  void runKey(SDL_Keycode key, KeyState state)
  {
    const size_t hash = makeHash(key, state);
    for (const auto &binding : bindings)
    {
      if (binding.first == hash)
      {
        binding.second();
        return;
      }
    }
  }

  void setHeld(SDL_Keycode key, bool held)
  {
    auto it = std::find(held_keys.begin(), held_keys.end(), key);
    if (held)
    {
      if (it == held_keys.end())
        held_keys.push_back(key);
      return;
    }

    if (it != held_keys.end())
      held_keys.erase(it);
  }

  bool isHeld(SDL_Keycode key) const
  {
    return std::find(held_keys.begin(), held_keys.end(), key) != held_keys.end();
  }
};