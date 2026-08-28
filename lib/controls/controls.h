#pragma once

#include "mouse.h"
#include "keyboard.h"
#include "lib/scene/scene.h"

class Controls
{

public:
  Keyboard keyboard;
  Mouse mouse;
  Scene *scene;

  void setup(Scene &scene) {
    this->scene = &scene;
    keyboard.setup(scene);
  };

  void handle() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      switch (event.type) {

        case SDL_EVENT_QUIT:
          scene->running = false;
          break;

        case SDL_EVENT_KEY_DOWN:
          keyboard.toggle(event.key.key, DOWN);
          break;

        case SDL_EVENT_KEY_UP:
          keyboard.toggle(event.key.key, UP);
          break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
          if (event.button.button == SDL_BUTTON_LEFT)
            mouse.down(event.button.x, event.button.y);
          break;

        case SDL_EVENT_MOUSE_MOTION:
          mouse.move(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
          break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
          if (event.button.button == SDL_BUTTON_LEFT)
            mouse.up(event.button.x, event.button.y);
          break;

        case SDL_EVENT_WINDOW_RESIZED:
          scene->window.resize(event.window.data1, event.window.data2);
          break;
        
        default:
          break;
      }
    }

    keyboard.process_held();
  }
};