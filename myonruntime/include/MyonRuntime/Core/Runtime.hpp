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
  MyonRuntime(MyonRuntimeConfig* p_RuntimeConfig);
  ~MyonRuntime();

  void Run();

private:
  bool m_ShouldClose;

  Window* m_Window;
};

}
