#version 450
#include "VertexAttributes.glslh"

layout (location = 0) out vec4 outColour;

void main() {
	outColour = inColour;

	gl_Position = vec4(inPos, 1.0f);
}