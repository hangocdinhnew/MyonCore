#pragma once

#include "GraphicsInstance.hpp"
#include "GraphicsSurface.hpp"
#include "GraphicsDevice.hpp"

namespace MyonR {

class Graphics {
public:
    Graphics(SDL_Window* p_Window);
    ~Graphics();

private:
    GraphicsInstance* m_Instance;
    GraphicsSurface* m_Surface;
    GraphicsDevice* m_Device;
};

} // namespace MyonR
