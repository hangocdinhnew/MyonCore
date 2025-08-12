#pragma once

#include <vulkan/vulkan.hpp>

namespace MyonR {

    class GraphicsDevice
    {
    public:
        GraphicsDevice(vk::Instance p_Instance);
        ~GraphicsDevice();

        vk::PhysicalDevice getPhysicalDevice() {
            return m_PhysicalDevice;
        }
        
    private:
        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_Device;

        vk::Queue m_GraphicsQueue;
    };
    
}
