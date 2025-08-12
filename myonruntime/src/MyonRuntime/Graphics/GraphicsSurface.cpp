#include <SDL3/SDL_Vulkan.h>

#include "MyonRuntime/Core/Log.hpp"
#include "MyonRuntime/Graphics/GraphicsSurface.hpp"

namespace MyonR {
    GraphicsSurface::GraphicsSurface(SDL_Window* p_Window, vk::Instance p_Instance) : m_Instance(p_Instance)
    {
        VkSurfaceKHR surface;
        bool result = SDL_Vulkan_CreateSurface(p_Window, m_Instance, nullptr, &surface);
        MR_CORE_ASSERT(result, "SDL3 - Failed to create surface with: {}!", SDL_GetError());

        m_Surface = vk::SurfaceKHR(surface);

        MR_CORE_INFO("Surface created!");
    }

    GraphicsSurface::~GraphicsSurface()
    {
        MR_CORE_INFO("Destroying Surface...");
        m_Instance.destroySurfaceKHR(m_Surface);
    }
}
