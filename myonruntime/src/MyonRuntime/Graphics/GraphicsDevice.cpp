#include "MyonRuntime/Core/Log.hpp"
#include "MyonRuntime/Graphics/GraphicsDevice.hpp"

namespace MyonR {

    std::vector<const char*> requiredDeviceExt = {
        vk::KHRSwapchainExtensionName,
        vk::KHRSpirv14ExtensionName,
        vk::KHRSynchronization2ExtensionName,
        vk::KHRCreateRenderpass2ExtensionName
    };

    GraphicsDevice::GraphicsDevice(vk::Instance p_Instance)
    {
        auto devices = p_Instance.enumeratePhysicalDevices(); 

        if (devices.empty())
            MR_DO_CORE_ASSERT("Cannot find GPUs with Vulkan support!");

        for (const auto& device : devices) {
            if (device.getProperties().apiVersion < VK_API_VERSION_1_3)
                continue;

            auto queueFamProps = device.getQueueFamilyProperties();

            bool hasGraphicsQueue = false;
            for (const auto& qfp : queueFamProps) {
                if (qfp.queueFlags & vk::QueueFlagBits::eGraphics) {
                    hasGraphicsQueue = !hasGraphicsQueue;
                    break;
                }
            }

            if (!hasGraphicsQueue)
                continue;

            auto availableDeviceExt = device.enumerateDeviceExtensionProperties();

            bool supportsAllRequiredExt = true;
            for (const char* requiredExt : requiredDeviceExt) {
                bool found = false;
                for (const auto& availExt : availableDeviceExt) {
                    if (strcmp(availExt.extensionName, requiredExt) == 0) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    supportsAllRequiredExt = false;
                    break;
                }
            }

            if (!supportsAllRequiredExt)
                continue;

            auto features = device.template getFeatures2<
                vk::PhysicalDeviceFeatures2,
                vk::PhysicalDeviceVulkan13Features,
                vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

            bool supportsRequiredFeatures =
                features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
                features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState;

            if (!supportsRequiredFeatures)
                continue;

            m_PhysicalDevice = device;
            break;
        }

        MR_CORE_INFO("Physical Device found!");

        auto queueFamilyProps = m_PhysicalDevice.getQueue;
    }

    GraphicsDevice::~GraphicsDevice()
    {
        MR_CORE_INFO("Destroying Device...");
    }

}
