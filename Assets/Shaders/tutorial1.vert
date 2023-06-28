#version 450 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inColour;

layout (location = 0) out vec4 outColour;

void main() {
	outColour = inColour;

	gl_Position = vec4(inPos, 1.0f);
}