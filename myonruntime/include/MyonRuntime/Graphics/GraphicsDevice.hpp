#pragma once

#include <vulkan/vulkan.hpp>

namespace MyonR {

    class GraphicsDevice
    {
    public:
        GraphicsDevice(vk::Instance p_Instance, vk::SurfaceKHR p_Surface);
        ~GraphicsDevice();

        vk::PhysicalDevice getPhysicalDevice() {
            return m_PhysicalDevice;
        }

        vk::Device getDevice() {
            return m_Device;
        }
        
    private:
        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_Device;

        vk::Queue m_GraphicsQueue;
        vk::Queue m_PresentQueue;
    };
    
}
