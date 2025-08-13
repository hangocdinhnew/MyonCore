#include "MyonRuntime/Core/Log.hpp"
#include "MyonRuntime/Graphics/GraphicsSwapChain.hpp"

namespace MyonR {

    GraphicsSwapChain::GraphicsSwapChain(SDL_Window* p_Window, vk::PhysicalDevice p_pDevice, vk::Device p_Device, vk::SurfaceKHR p_Surface)
        : m_Device(p_Device)
    {
        auto surfaceCapabilities = p_pDevice.getSurfaceCapabilitiesKHR(p_Surface);
        auto surfaceFormats = p_pDevice.getSurfaceFormatsKHR(p_Surface);
        
        for (const auto& format : surfaceFormats) {
            if (format.format == vk::Format::eB8G8R8A8Srgb &&
                format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            {
                m_SwapChainImageFormat = format.format;
                break;
            }
        }

        bool assign = true;
        if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            m_SwapChainExtent = surfaceCapabilities.currentExtent;
            assign = false;
        }

        if (assign) {
            int width, height;
            SDL_GetWindowSizeInPixels(p_Window, &width, &height);

            if (width < surfaceCapabilities.minImageExtent.width) {
                width = surfaceCapabilities.minImageExtent.width;
            } else if (width > surfaceCapabilities.maxImageExtent.width) {
                width = surfaceCapabilities.maxImageExtent.width;
            }

            if (height < surfaceCapabilities.minImageExtent.height) {
                height = surfaceCapabilities.minImageExtent.height;
            } else if (height > surfaceCapabilities.maxImageExtent.height) {
                height = surfaceCapabilities.maxImageExtent.height;
            }

            m_SwapChainExtent.width = width;
            m_SwapChainExtent.height = height;
        }

        uint32_t minImageCount = 3;
        if (minImageCount < surfaceCapabilities.minImageCount)
            minImageCount = surfaceCapabilities.minImageCount;

        if (surfaceCapabilities.maxImageCount > 0 && minImageCount > surfaceCapabilities.maxImageCount)
            minImageCount = surfaceCapabilities.maxImageCount;

        vk::SwapchainCreateInfoKHR createInfo{};
        createInfo.surface = p_Surface;
        createInfo.minImageCount = minImageCount;
        createInfo.imageFormat = m_SwapChainImageFormat;
        createInfo.imageColorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
        createInfo.imageExtent = m_SwapChainExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        createInfo.preTransform = surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = vk::PresentModeKHR::eFifo;
        createInfo.clipped = true;

        m_SwapChain = m_Device.createSwapchainKHR(createInfo);
        m_SwapChainImages = m_Device.getSwapchainImagesKHR(m_SwapChain);

        MR_CORE_INFO("Swapchain successfully created");
    }

    GraphicsSwapChain::~GraphicsSwapChain() {
        MR_CORE_INFO("Destroying Swapchain...");
        m_Device.destroySwapchainKHR(m_SwapChain);
    }
    
}
