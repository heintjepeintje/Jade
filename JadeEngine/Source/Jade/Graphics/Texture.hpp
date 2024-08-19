#pragma once

#include "./../Platform/NativeTexture.hpp"
#include "./GraphicsContext.hpp"

namespace Jade {

	class Texture2D {
	public:
		static Texture2D Create(const GraphicsContext &context, uint32_t width, uint32_t height, uint32_t bpp, void *data) {
			Texture2D texture;
			texture.m_NativeHandle = Native::NativeTexture2D::Create(
				context.GetNativeHandle(),
				width, height, bpp,
				data
			);
			return texture;
		}

	public:
		Texture2D() = default;
		~Texture2D() = default;

		inline Ref<Native::NativeTexture2D> GetNativeHandle() const { return m_NativeHandle; }

	private:
		Ref<Native::NativeTexture2D> m_NativeHandle;
	};

}