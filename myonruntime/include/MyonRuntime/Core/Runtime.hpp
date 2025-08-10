#pragma once

#include "MyonRuntime/Core/Window.hpp"

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

  void Run();

  Window *getWindow() { return m_Window; }

private:
  bool m_ShouldClose;

  Window *m_Window;
};

} // namespace MyonR
