#version 450

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aColor;

layout(location = 0) out vec4 oColor;

void main() {
	oColor = aColor;
    gl_Position = vec4(aPosition, 1.0);
}