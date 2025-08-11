#include "MyonRuntime/Core/Log.hpp"
#include "MyonRuntime/Graphics/Graphics.hpp"

namespace MyonR {

    Graphics::Graphics()
    {
        MR_CORE_INFO("Creating Graphics API!");
        m_Instance = new GraphicsInstance();
    }

    Graphics::~Graphics()
    {
        MR_CORE_INFO("Destroying Graphics API!");
        delete m_Instance;
    }

}
