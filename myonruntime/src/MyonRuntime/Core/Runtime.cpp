#include "MyonRuntime/Core/Log.hpp"
#include "MyonRuntime/Core/Runtime.hpp"

namespace Myon {

MyonRuntime::MyonRuntime() {
  Log::Init();
  MR_CORE_INFO("Engine initialized");
}

}
