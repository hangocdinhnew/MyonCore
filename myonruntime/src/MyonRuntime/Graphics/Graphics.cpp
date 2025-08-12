#include "MyonRuntime/Core/Log.hpp"
#include "MyonRuntime/Graphics/Graphics.hpp"

namespace MyonR {

    Graphics::Graphics(SDL_Window* p_Window)
    {
        MR_CORE_INFO("Creating Graphics API!");
        m_Instance = new GraphicsInstance();
        m_Surface = new GraphicsSurface(p_Window, m_Instance->getInstance());
        m_Device = new GraphicsDevice(m_Instance->getInstance(), m_Surface->getSurface());
    }

    Graphics::~Graphics()
    {
        MR_CORE_INFO("Destroying Graphics API!");
        delete m_Device;
        delete m_Surface;
        delete m_Instance;
    }

}
