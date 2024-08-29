#pragma once

#include "./GraphicsContext.hpp"
#include "./../Platform/NativeRenderPass.hpp"

namespace Jade {

	class RenderPass {
	public:
		static RenderPass Create(const GraphicsContext &context) {
			RenderPass pass;
			pass.m_NativeHandle = Native::NativeRenderPass::Create(context.GetNativeHandle());
			return pass;
		}
		
	public:
		RenderPass() = default;
		~RenderPass() = default;
		
		inline Ref<Native::NativeRenderPass> GetNativeHandle() const { return m_NativeHandle; }
		
	private:
		Ref<Native::NativeRenderPass> m_NativeHandle;
	};

}