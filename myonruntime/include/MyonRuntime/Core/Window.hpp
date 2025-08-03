#pragma once

// clang-format off
#include <string>

#include <SDL3/SDL.h>
// clang-format on

namespace MyonR {

class Window {
public:
  Window(std::string name, int width, int height);
  ~Window();

  SDL_Window *getWindow() { return m_Window; }

private:
  SDL_Window *m_Window;
};

} // namespace Myon
