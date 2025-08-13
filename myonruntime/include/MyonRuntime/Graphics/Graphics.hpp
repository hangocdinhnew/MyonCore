#pragma once

#include "GraphicsInstance.hpp"
#include "GraphicsSurface.hpp"
#include "GraphicsDevice.hpp"
#include "GraphicsSwapChain.hpp"

namespace MyonR {

class Graphics {
public:
    Graphics(SDL_Window* p_Window);
    ~Graphics();

private:
    GraphicsInstance* m_Instance;
    GraphicsSurface* m_Surface;
    GraphicsDevice* m_Device;
    GraphicsSwapChain* m_SwapChain;
};

} // namespace MyonR
