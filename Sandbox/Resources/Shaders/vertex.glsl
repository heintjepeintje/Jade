#version 450

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aColor;
layout(location = 3) in uint aID;

layout(location = 0) out vec4 oColor;
layout(location = 1) out vec2 oUV;

layout(binding = 0) uniform CameraData {
	mat4 Projection;
	mat4 View;
} uCameraData;

layout(binding = 1) uniform ObjectData {
	mat4 Model[16];
} uObjectData;

void main() {
	oColor = aColor;
	oUV = aUV;
    gl_Position = uCameraData.Projection * uCameraData.View * uObjectData.Model[aID] * vec4(aPosition, 1.0);
}