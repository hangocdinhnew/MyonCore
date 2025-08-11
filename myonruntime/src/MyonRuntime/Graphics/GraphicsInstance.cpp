// clang-format offp
#include <cstring>

#include "SDL3/SDL_vulkan.h"

#include "MyonRuntime/Core/Log.hpp"
#include "MyonRuntime/Graphics/GraphicsInstance.hpp"
// clang-format on

namespace MyonR {

    static std::vector<const char *> layers = {"VK_LAYER_KHRONOS_validation"};

    static std::vector<const char *> getRequiredExtensions()
    {
        uint32_t extensionCount = 0;
        auto extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

        std::vector extensionsvector(extensions, extensions + extensionCount);
        extensionsvector.push_back(vk::EXTDebugUtilsExtensionName);

        return extensionsvector;
    }

    static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(
        vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
        vk::DebugUtilsMessageTypeFlagsEXT type,
        const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData, void*)
    {
        switch (severity) {
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose:
            MR_CORE_DEBUG(pCallbackData->pMessage);
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
            MR_CORE_WARN(pCallbackData->pMessage);
            break;
        case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
            MR_CORE_ERROR(pCallbackData->pMessage);
            break;
        default:
            MR_CORE_INFO("Unknown log level - {}", pCallbackData->pMessage);
            break;
        }
        
        return vk::False;
    }

    GraphicsInstance::GraphicsInstance()
    {
        vk::ApplicationInfo appInfo{};
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_3;
        appInfo.pEngineName = "MyonEngine";
        appInfo.sType = vk::StructureType::eApplicationInfo;

        auto requiredExtensions = getRequiredExtensions();

        vk::InstanceCreateInfo createInfo{};
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledLayerCount = layers.size();
        createInfo.ppEnabledLayerNames = layers.data();
        createInfo.enabledExtensionCount = requiredExtensions.size();
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        for (int i = 0; i < requiredExtensions.size(); ++i)
        {
            if (strcmp(requiredExtensions[i], VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) == 0) {
                createInfo.flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
                break;
            }
        }
        
        createInfo.sType = vk::StructureType::eInstanceCreateInfo;

        m_Instance = vk::createInstance(createInfo);
        MR_CORE_ASSERT(m_Instance, "Failed to create Vulkan Instance");

        m_Dispatcher = vk::detail::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);

        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                                                            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                                            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);

        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                                           vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                                           vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

        vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{};
        debugUtilsMessengerCreateInfoEXT.messageSeverity = severityFlags;
        debugUtilsMessengerCreateInfoEXT.messageType = messageTypeFlags;
        debugUtilsMessengerCreateInfoEXT.pfnUserCallback = &debugCallback;

        m_DebugMessenger = m_Instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT, nullptr, m_Dispatcher);
        MR_CORE_ASSERT(m_DebugMessenger, "Failed to create debug messenger!");

        MR_CORE_INFO("Successfully created Vulkan Instance!");
    }

    GraphicsInstance::~GraphicsInstance()
    {
        MR_CORE_INFO("Destroying instance...");
        m_Instance.destroyDebugUtilsMessengerEXT(m_DebugMessenger, nullptr, m_Dispatcher);
        m_Instance.destroy();
    }

} // namespace MyonR
