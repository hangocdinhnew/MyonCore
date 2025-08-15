// clang-format off
#include <MyonRuntime/Core/Log.hpp>
#include <MyonRuntime/Core/Window.hpp>
// clang-format on

namespace MyonR {

Window::Window(std::string name, int width, int height) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    MR_CORE_ERROR("Failed to initialize SDL Video!");

    MR_DO_CORE_ASSERT("With error: {}", SDL_GetError());
  }

  m_Window = SDL_CreateWindow(name.c_str(), width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

  MR_CORE_ASSERT(m_Window, "Failed to create Window! With error: {}", SDL_GetError());

  MR_CORE_INFO("Window created!");
}

Window::~Window() {
  MR_CORE_INFO("Destroying Window...");

  SDL_DestroyWindow(m_Window);
}

} // namespace Myon
