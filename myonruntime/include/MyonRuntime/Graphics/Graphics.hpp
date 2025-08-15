#pragma once

#include <SDL3/SDL.h>
#include <VkBootstrap.h>

namespace MyonR {

class Graphics
{
public:
    Graphics(SDL_Window* p_Window);
    ~Graphics();

    vkb::Instance* getInstance()
    {
        return &m_Instance;
    }

    vkb::InstanceDispatchTable* getInstDisp() {
        return &m_InstDisp;
    }

    VkSurfaceKHR* getSurface() {
        return &m_Surface;
    }

    vkb::Device* getDevice() {
        return &m_Device;
    }

    vkb::DispatchTable* getDisp() {
        return &m_Disp;
    }

    vkb::Swapchain* getSwapchain() {
        return &m_Swapchain;
    }

    void createSwapchain();
    void destroySwapchain();
    
private:
    vkb::Instance m_Instance;
    vkb::InstanceDispatchTable m_InstDisp;

    VkSurfaceKHR m_Surface;

    vkb::Device m_Device;
    vkb::DispatchTable m_Disp;

    vkb::Swapchain m_Swapchain;

    VkQueue m_GraphicsQueue;
    VkQueue m_PresentQueue;
};

} // namespace MyonR
