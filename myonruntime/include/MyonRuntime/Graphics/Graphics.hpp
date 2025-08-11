#pragma once

#include "GraphicsInstance.hpp"

namespace MyonR {

class Graphics {
public:
  Graphics();
  ~Graphics();

private:
  GraphicsInstance* m_Instance;
};

} // namespace MyonR
