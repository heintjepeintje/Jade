#pragma once

#include "./../System/Window.hpp"
#include "./../Platform/NativeGraphicsContext.hpp"

namespace Jade {
	
	class GraphicsContext {
	public:
		GraphicsContext() = default;
		GraphicsContext(const Window &window) {
			m_NativeHandle = Native::NativeGraphicsContext::Create(window.GetNativeHandle());
		}
		
		~GraphicsContext() = default;
		
		inline Ref<Native::NativeGraphicsContext> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeGraphicsContext> m_NativeHandle;
	};
	
	class SwapChain {
	public:
		SwapChain() = default;
		SwapChain(const GraphicsContext &context, uint32_t bufferCount) {
			m_NativeHandle = Native::NativeSwapChain::Create(context.GetNativeHandle(), bufferCount);
		}
		~SwapChain() = default;
		
		inline void Present() { m_NativeHandle->Present(); }
		inline void Resize(uint32_t width, uint32_t height) { m_NativeHandle->Resize(width, height); }
		
		inline Ref<Native::NativeSwapChain> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeSwapChain> m_NativeHandle;
	};

}