#pragma once

#include "./GraphicsContext.hpp"
#include "./../Platform/NativeRenderer.hpp"
#include "./Mesh.hpp"
#include "./RenderPass.hpp"
#include "./Framebuffer.hpp"
#include "./Shader.hpp"

namespace Jade {

	class RendererAPI {
	public:
		RendererAPI() = default;
		RendererAPI(const GraphicsContext &context, const SwapChain &swapChain) {
			m_NativeHandle = Native::NativeRendererAPI::Create(context.GetNativeHandle(), swapChain.GetNativeHandle());
		}
		
		~RendererAPI() = default;
		
		inline void Begin() {
			m_NativeHandle->Begin();
		}
		
		inline void BeginRender(const RenderPipeline &pipeline) {
			m_NativeHandle->BeginRender(pipeline.GetNativeHandle());
		}
		
		inline void Clear(float r, float g, float b, float a) {
			m_NativeHandle->Clear(r, g, b, a);
		}
		
		inline void DrawIndexed(const VertexBuffer &vbo, const IndexBuffer &ibo) { 
			m_NativeHandle->DrawIndexed(
				vbo.GetNativeHandle(),
				ibo.GetNativeHandle()
			);
		}
		
		inline void EndRender() {
			m_NativeHandle->EndRender();
		}
		
		inline void End() {
			m_NativeHandle->End();
		}
		
	private:
		Ref<Native::NativeRendererAPI> m_NativeHandle;
	};

}