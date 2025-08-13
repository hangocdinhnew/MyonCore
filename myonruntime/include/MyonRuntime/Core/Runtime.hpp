#pragma once

#include "MyonRuntime/Core/Window.hpp"
#include "MyonRuntime/Graphics/Graphics.hpp"

namespace MyonR {

struct MyonRuntimeConfig {
  std::string name;
  int width;
  int height;
};

class MyonRuntime {
public:
  MyonRuntime(MyonRuntimeConfig *p_RuntimeConfig);
  ~MyonRuntime();

  void PollEvents();

  Window *getWindow() { return m_Window; }

  bool shouldClose;

private:
  Window *m_Window;
  Graphics *m_Graphics;
};

} // namespace MyonR
