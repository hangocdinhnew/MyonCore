#pragma once

#include <SDL3/SDL.h>

namespace MyonR {

class Graphics {
public:
    Graphics(SDL_Window* p_Window);
    ~Graphics();

    void acquireCmdBuffer();
    void acquireTexture(uint32_t* swapchain_texture_width = nullptr,
                        uint32_t* swapchain_texture_height = nullptr);

    void fillColorTargetInfo(SDL_GPUColorTargetInfo* color_target_info);

    SDL_GPURenderPass* beginRenderPass(const SDL_GPUColorTargetInfo* color_target_info,
                                       uint32_t num_color_targets,
                                       const SDL_GPUDepthStencilTargetInfo *depth_stencil_target_info = nullptr);
    void endRenderPass(SDL_GPURenderPass* renderpass);

    void submit();

private:
    SDL_Window* m_Window;

    SDL_GPUDevice* m_Device;
    SDL_GPUCommandBuffer* m_CmdBuffer;
    SDL_GPUTexture* m_ScTexture;
};

} // namespace MyonR
