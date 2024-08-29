#pragma once

#include "./../System/Window.hpp"
#include "./../Platform/NativeGraphicsContext.hpp"

namespace Jade {
	
	class GraphicsContext {
	public:
		static GraphicsContext Create(const Window &window) {
			GraphicsContext context;
			context.m_NativeHandle = Native::NativeGraphicsContext::Create(window.GetNativeHandle());
			return context;
		}

	public:
		GraphicsContext() = default;		
		~GraphicsContext() = default;
		
		inline Ref<Native::NativeGraphicsContext> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeGraphicsContext> m_NativeHandle;
	};
	
	class SwapChain {
	public:
		static SwapChain Create(const GraphicsContext &context, uint32_t bufferCount) {
			SwapChain swapChain;
			swapChain.m_NativeHandle = Native::NativeSwapChain::Create(context.GetNativeHandle(), bufferCount);
			return swapChain;
		}
	public:
		SwapChain() = default;
		~SwapChain() = default;
		
		inline void Present() { m_NativeHandle->Present(); }
		inline void Resize(uint32_t width, uint32_t height) { m_NativeHandle->Resize(width, height); }
		
		inline Ref<Native::NativeSwapChain> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeSwapChain> m_NativeHandle;
	};

}