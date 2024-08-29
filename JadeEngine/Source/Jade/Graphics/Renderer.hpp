#pragma once

#include "./GraphicsContext.hpp"
#include "./../Platform/NativeRenderer.hpp"
#include "./Buffer.hpp"
#include "./RenderPass.hpp"
#include "./Framebuffer.hpp"
#include "./Shader.hpp"
#include "./RenderPipeline.hpp"

namespace Jade {

	class RendererAPI {
	public:
		static RendererAPI Create(const GraphicsContext &context, const SwapChain &swapChain) {
			RendererAPI api;
			api.m_NativeHandle = Native::NativeRendererAPI::Create(context.GetNativeHandle(), swapChain.GetNativeHandle());
			return api;
		}
	
	public:
		RendererAPI() = default;	
		~RendererAPI() = default;
		
		inline void Begin() { m_NativeHandle->Begin(); }
		inline void End() { m_NativeHandle->End(); }
		inline void BeginRender(const RenderPipeline &pipeline) { m_NativeHandle->BeginRender(pipeline.GetNativeHandle()); }
		inline void EndRender() { m_NativeHandle->EndRender(); }
		inline void Clear(float r, float g, float b, float a) { m_NativeHandle->Clear(r, g, b, a); }
		inline void DrawIndexed(const VertexBuffer &vbo, const IndexBuffer &ibo) { m_NativeHandle->DrawIndexed(vbo.GetNativeHandle(), ibo.GetNativeHandle()); }
		
	private:
		Ref<Native::NativeRendererAPI> m_NativeHandle;
	};

}