#pragma once

// clang-format off
#include <SDL3/SDL.h>

#include <vulkan/vulkan.hpp>
// clang-format on

namespace MyonR {

    class GraphicsInstance
    {
    public:
        GraphicsInstance();
        ~GraphicsInstance();

        vk::Instance getInstance() {
            return m_Instance;
        }
      
    private:
        vk::Instance m_Instance;
        vk::DebugUtilsMessengerEXT m_DebugMessenger;

        vk::detail::DispatchLoaderDynamic m_Dispatcher;
    };

}
