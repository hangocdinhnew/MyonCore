#pragma once

#include <SDL3/SDL.h>
#include <vulkan/vulkan.hpp>

namespace MyonR {

    class GraphicsSurface
    {
    public:
        GraphicsSurface(SDL_Window* p_Window, vk::Instance p_Instance);
        ~GraphicsSurface();

        vk::SurfaceKHR getSurface() {
            return m_Surface;
        }
        
    private:
        vk::SurfaceKHR m_Surface;
        
        vk::Instance m_Instance;
    };
    
}
