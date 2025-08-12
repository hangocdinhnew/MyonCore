#include "MyonRuntime/Core/Log.hpp"
#include "MyonRuntime/Graphics/GraphicsDevice.hpp"

namespace MyonR {

    std::vector<const char*> requiredDeviceExt = {
        vk::KHRSwapchainExtensionName,
        vk::KHRSpirv14ExtensionName,
        vk::KHRSynchronization2ExtensionName,
        vk::KHRCreateRenderpass2ExtensionName
    };

    GraphicsDevice::GraphicsDevice(vk::Instance p_Instance, vk::SurfaceKHR p_Surface)
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

            for (const auto &availExt : availableDeviceExt)
            {
                if (strcmp(availExt.extensionName, "VK_KHR_portability_subset") != 0) {
                    requiredDeviceExt.push_back("VK_KHR_portability_subset");
                    break;
                }
            }

            bool supportsAllRequiredExt = true;
            for (const char* requiredExt : requiredDeviceExt) {
                bool found = false;
                for (const auto& availExt : availableDeviceExt)
                {
                    if (strcmp(availExt.extensionName, requiredExt) == 0)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
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

        auto queueFamProps = m_PhysicalDevice.getQueueFamilyProperties();

        int queueFamIDX = 0;
        for (int i = 0; i < queueFamProps.size(); ++i)
        {
            if (queueFamProps[i].queueFlags & vk::QueueFlagBits::eGraphics)
            {
                queueFamIDX = i;
                break;
            }
        }
        
        int presentIDX = 0;
        if (m_PhysicalDevice.getSurfaceSupportKHR(queueFamIDX, p_Surface))
        {
            presentIDX = queueFamIDX;
        }
        else
        {
            for (uint32_t i = 0; i < queueFamProps.size(); ++i)
            {
                if ((queueFamProps[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
                    m_PhysicalDevice.getSurfaceSupportKHR(i, p_Surface))
                {
                    queueFamIDX = i;
                    presentIDX = i;
                    break;
                }
            }

            if (presentIDX == queueFamProps.size())
            {
                for (uint32_t i = 0; i < queueFamProps.size(); ++i)
                {
                    if (m_PhysicalDevice.getSurfaceSupportKHR(i, p_Surface))
                    {
                        presentIDX = i;
                        break;
                    }
                }
            }
        }
 
        if (queueFamIDX == queueFamProps.size() || presentIDX == queueFamProps.size())
        {
            throw std::runtime_error("Could not find a queue family that supports graphics or present");
        }

        auto features = m_PhysicalDevice.getFeatures2();
        vk::PhysicalDeviceVulkan13Features vulkan13Features;
        vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT extendedDynamicStateFeatures;
        vulkan13Features.dynamicRendering = vk::True;
        extendedDynamicStateFeatures.extendedDynamicState = vk::True;
        vulkan13Features.pNext = &extendedDynamicStateFeatures;
        features.pNext = &vulkan13Features;

        float queuePriority = 0.0f;
        vk::DeviceQueueCreateInfo deviceQueueCreateInfo{};
        deviceQueueCreateInfo.queueFamilyIndex = queueFamIDX;
        deviceQueueCreateInfo.queueCount = 1;
        deviceQueueCreateInfo.pQueuePriorities = &queuePriority;

        vk::DeviceCreateInfo deviceCreateInfo{};
        deviceCreateInfo.pNext = &features;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExt.size());
        deviceCreateInfo.ppEnabledExtensionNames = requiredDeviceExt.data();

        m_Device = m_PhysicalDevice.createDevice(deviceCreateInfo);
        m_GraphicsQueue = m_Device.getQueue(queueFamIDX, 0);
        m_PresentQueue = m_Device.getQueue(presentIDX, 0);

        MR_CORE_INFO("Logical Device created!");
    }

    GraphicsDevice::~GraphicsDevice()
    {
        MR_CORE_INFO("Destroying Device...");

        m_Device.destroy();
    }

}
