#include "MyonRuntime/Core/Log.hpp"
#include "MyonRuntime/Graphics/Graphics.hpp"

namespace MyonR {

    Graphics::Graphics(SDL_Window* p_Window) :
        m_Window(p_Window)
    {
        m_Device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV,
                                       true,
                                       nullptr);
        MR_CORE_ASSERT(m_Device, "Failed to create Device! Error: {}", SDL_GetError());

        if (!SDL_ClaimWindowForGPUDevice(m_Device, m_Window))
            MR_DO_CORE_ASSERT("Failed to claim window for GPU Device! Error: {}", SDL_GetError());

        MR_CORE_INFO("Created Graphics API!");
    }

    Graphics::~Graphics()
    {
        MR_CORE_INFO("Destroying Graphics API!");

        SDL_ReleaseWindowFromGPUDevice(m_Device, m_Window);
        SDL_DestroyGPUDevice(m_Device);
    }

    void Graphics::acquireCmdBuffer()
    {
        m_CmdBuffer = SDL_AcquireGPUCommandBuffer(m_Device);
        MR_CORE_ASSERT(m_CmdBuffer, "Failed to acquire Command Buffer! Error: {}", SDL_GetError());
    }

    void Graphics::acquireTexture(uint32_t* swapchain_texture_width,
                                  uint32_t* swapchain_texture_height)
    {
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(m_CmdBuffer,
                                                   m_Window,
                                                   &m_ScTexture,
                                                   swapchain_texture_width,
                                                   swapchain_texture_height))
            MR_CORE_ASSERT(m_CmdBuffer, "Failed to acquire Swapchain Texture! Error: {}", SDL_GetError());

        MR_CORE_ASSERT(m_CmdBuffer, "Unreachable code!");
    }

    SDL_GPUColorTargetInfo Graphics::createColorTargetInfo()
    {
        SDL_GPUColorTargetInfo colorTargetInfo{};
        colorTargetInfo.texture = m_ScTexture;

        return colorTargetInfo;
    }

    SDL_GPURenderPass* Graphics::beginRenderPass(const SDL_GPUColorTargetInfo* color_target_info,
                                                 uint32_t num_color_targets,
                                                 const SDL_GPUDepthStencilTargetInfo *depth_stencil_target_info)
    {
        MR_CORE_ASSERT(color_target_info->texture, "Please use fillColorTargetInfo on targetInfo!");

        return SDL_BeginGPURenderPass(m_CmdBuffer, color_target_info, num_color_targets,
                                      depth_stencil_target_info);
    }

    void Graphics::endRenderPass(SDL_GPURenderPass* renderpass)
    {
        SDL_EndGPURenderPass(renderpass);
    }

    void Graphics::submit()
    {
        SDL_SubmitGPUCommandBuffer(m_CmdBuffer);
    }

    void Graphics::setSwapchainParameters(SDL_GPUSwapchainComposition swapchain_composition,
                                          SDL_GPUPresentMode present_mode)
    {
        SDL_SetGPUSwapchainParameters(m_Device, m_Window, swapchain_composition, present_mode);
    }
}
