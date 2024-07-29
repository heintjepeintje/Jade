#pragma once

#include "./GraphicsContext.hpp"
#include "./../Platform/NativeRenderPass.hpp"

namespace Jade {

	class RenderPass {
	public:
		RenderPass() = default;
		RenderPass(const GraphicsContext &context) {
			m_NativeHandle = Native::NativeRenderPass::Create(context.GetNativeHandle());
		}
		~RenderPass() = default;
		
		inline Ref<Native::NativeRenderPass> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeRenderPass> m_NativeHandle;
	};

}