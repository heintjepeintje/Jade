#pragma once

#include "./../Core.hpp"
#include "./NativeGraphicsContext.hpp"
#include "./NativeMesh.hpp"
#include "./NativeRenderPass.hpp"
#include "./NativeFramebuffer.hpp"
#include "./NativeShader.hpp"

namespace Jade {

	namespace Native {

		class NativeRendererAPI {
		public:
			static Ref<NativeRendererAPI> Create(const Ref<NativeGraphicsContext> &context, const Ref<NativeSwapChain> &swapChain);
		public:
			NativeRendererAPI() = default;
			virtual ~NativeRendererAPI() = default;
			
			virtual void Begin() = 0;
			virtual void BeginRender(const Ref<NativeRenderPipeline> &pipeline) = 0;
			virtual void Clear(float r, float g, float b, float a) = 0;
			virtual void DrawIndexed(const Ref<NativeVertexBuffer> &vbo, const Ref<NativeIndexBuffer> &ibo) = 0;
			virtual void EndRender() = 0;
			virtual void End() = 0;
		};
	
	}

}