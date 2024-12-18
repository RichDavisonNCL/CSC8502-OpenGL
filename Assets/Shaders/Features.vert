#version 450
#include "VertexAttributes.glslh"
#include "Camera.glslh"

layout (location = 0) out vec4 outColour;
layout (location = 1) out vec2 outUV;

layout(location = 0) uniform Camera cam;

void main() {
	outColour	= inColour;
	outUV		= inUV;

	vec4 vertexPos = vec4(inPos, 1.0f);

	gl_Position = cam.projMatrix * cam.viewMatrix * vec4(inPos, 1.0f);
}