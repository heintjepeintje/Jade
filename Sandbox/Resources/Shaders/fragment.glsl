#version 450

layout(location = 0) in vec4 iColor;
layout(location = 1) in vec2 iUV;

layout(binding = 2) uniform sampler2D surfaceImage;

layout(location = 0) out vec4 oColor;

void main() {
    oColor = texture(surfaceImage, iUV) * iColor;
}