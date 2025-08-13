#pragma once

#include <SDL3/SDL.h>
#include <vulkan/vulkan.hpp>

namespace MyonR {

    class GraphicsSwapChain {
    public:
        GraphicsSwapChain(SDL_Window* p_Window, vk::PhysicalDevice p_pDevice, vk::Device p_Device, vk::SurfaceKHR p_Surface);
        ~GraphicsSwapChain();

        vk::SwapchainKHR getSwapChain() {
            return m_SwapChain;
        }

        std::vector<vk::Image> getSwapChainImages() {
            return m_SwapChainImages;
        }

        vk::Format getSwapChainImageFormat() {
            return m_SwapChainImageFormat;
        }

        vk::Extent2D getSwapChainExtent() {
            return m_SwapChainExtent;
        }

    private:
        vk::Device m_Device;
        
        vk::SwapchainKHR m_SwapChain;
        std::vector<vk::Image> m_SwapChainImages;
        vk::Format m_SwapChainImageFormat;
        vk::Extent2D m_SwapChainExtent;
    };
    
}
