#pragma once

#include "./../Platform/NativeFramebuffer.hpp"
#include "./GraphicsContext.hpp"
#include "./RenderPass.hpp"

namespace Jade {

	class Framebuffer {
	public:
		Framebuffer() = default;
		Framebuffer(const GraphicsContext &context, const SwapChain &swapChain, const RenderPass &renderPass) {
			m_NativeHandle = Native::NativeFramebuffer::Create(
				context.GetNativeHandle(),
				swapChain.GetNativeHandle(),
				renderPass.GetNativeHandle()
			);
		}
		~Framebuffer() = default;
		
		inline Ref<Native::NativeFramebuffer> GetNativeHandle() const { return m_NativeHandle; }		
		
	private:
		Ref<Native::NativeFramebuffer> m_NativeHandle;	
	};

}