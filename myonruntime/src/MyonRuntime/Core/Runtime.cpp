#include "MyonRuntime/Core/Runtime.hpp"
#include "MyonRuntime/Core/Log.hpp"

namespace MyonR {

MyonRuntime::MyonRuntime(MyonRuntimeConfig *p_RuntimeConfig) {
  Log::Init();
  m_Window = new Window(p_RuntimeConfig->name, p_RuntimeConfig->width,
                        p_RuntimeConfig->height);

  MR_CORE_INFO("Engine initialized");
}

MyonRuntime::~MyonRuntime() {
  MR_CORE_INFO("Shutting down Engine...");

  delete m_Window;
  SDL_Quit();
}

} // namespace MyonR
