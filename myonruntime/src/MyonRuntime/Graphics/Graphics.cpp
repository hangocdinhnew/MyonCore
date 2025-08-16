#include <SDL3/SDL_vulkan.h>

#include "MyonRuntime/Core/Log.hpp"
#include "MyonRuntime/Graphics/Graphics.hpp"

namespace MyonR {
    std::vector<const char*> requiredDeviceExtension = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_SPIRV_1_4_EXTENSION_NAME,
        VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
        VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME
    };
    
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                                        VkDebugUtilsMessageTypeFlagsEXT type,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void*)
    {
        switch (type) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            MR_CORE_ERROR(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            MR_CORE_WARN(pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            MR_CORE_DEBUG(pCallbackData->pMessage);
            break;
        default:
            MR_CORE_INFO("Unknown type: {}", pCallbackData->pMessage);
            break;
        }

        return VK_FALSE;
    }

    Graphics::Graphics(SDL_Window* p_Window)
    {
        vkb::InstanceBuilder instance_builder;
        instance_builder.request_validation_layers();
        instance_builder.set_debug_callback(debugCallback);
        instance_builder.require_api_version(1, 3);

        auto instance_ret = instance_builder.build();
        MR_CORE_ASSERT(instance_ret, "Failed to create instance! Error: {}", instance_ret.error().message());
        m_Instance = instance_ret.value();

        MR_CORE_INFO("Created Instance!");

        m_InstDisp = m_Instance.make_table();

        if (!SDL_Vulkan_CreateSurface(p_Window, m_Instance, nullptr, &m_Surface))
        {
            MR_DO_CORE_ASSERT("Failed to create Surface! Error: {}", SDL_GetError());
        }

        MR_CORE_INFO("Created Surface!");

        vkb::PhysicalDeviceSelector phys_device_selector(m_Instance);
        phys_device_selector.add_required_extensions(requiredDeviceExtension);
        
        VkPhysicalDeviceVulkan13Features vkFeature13{};
        vkFeature13.synchronization2 = VK_TRUE;
        vkFeature13.dynamicRendering = VK_TRUE;

        VkPhysicalDeviceExtendedDynamicState2FeaturesEXT extendeddynamicstate{};
        extendeddynamicstate.extendedDynamicState2 = VK_TRUE;
        extendeddynamicstate.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_2_FEATURES_EXT;
        vkFeature13.pNext = &extendeddynamicstate;
        phys_device_selector.set_required_features_13(vkFeature13);

        phys_device_selector.set_surface(m_Surface);
        
        auto phys_device_ret = phys_device_selector.select();
        MR_CORE_ASSERT(phys_device_ret, "Failed to enumerate physical device! Error: {}", phys_device_ret.error().message());
        vkb::PhysicalDevice physical_device = phys_device_ret.value();

        vkb::DeviceBuilder device_builder{physical_device};
        auto device_ret = device_builder.build();
        MR_CORE_ASSERT(device_ret, "Failed to create device! Error: {}", device_ret.error().message());
        m_Device = device_ret.value();

        MR_CORE_INFO("Created Device!");

        createSwapchain();
        MR_CORE_INFO("Created Swapchain!");

        auto gq = m_Device.get_queue(vkb::QueueType::graphics);
        MR_CORE_ASSERT(gq, "Failed to get Graphics Queue! Error: {}", gq.error().message());
        m_GraphicsQueue = gq.value();

        auto pq = m_Device.get_queue(vkb::QueueType::present);
        MR_CORE_ASSERT(pq, "Failed to get Present Queue! Error: {}", pq.error().message());
        m_PresentQueue = pq.value();
        MR_CORE_INFO("Got queue!");

        MR_CORE_INFO("Created Graphics API!");
    }

    Graphics::~Graphics()
    {
        MR_CORE_INFO("Destroying Graphics API...");
        
        destroySwapchain();
        vkb::destroy_device(m_Device);
        vkb::destroy_surface(m_Instance, m_Surface);
        vkb::destroy_instance(m_Instance);
    }

    void Graphics::createSwapchain()
    {
        vkb::SwapchainBuilder swapchain_builder{m_Device};
        auto swap_ret = swapchain_builder.set_old_swapchain(m_Swapchain).build();
        MR_CORE_ASSERT(swap_ret, "Failed to create Swapchain! Error: {}", swap_ret.error().message());

        m_Swapchain = swap_ret.value();
    }

    void Graphics::destroySwapchain()
    {
        vkb::destroy_swapchain(m_Swapchain);
    }
}
