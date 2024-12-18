#version 450 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inColour;

layout (location = 0) out vec4 outColour;
layout (location = 1) out vec2 outUV;

uniform mat4 modelMatrix	= mat4(1.0f);
uniform mat4 viewMatrix		= mat4(1.0f);
uniform mat4 projMatrix		= mat4(1.0f);

uniform vec4 objectColour	= vec4(1.0f, 1.0f, 1.0f, 1.0f);

void main() {
	outColour	= objectColour;

	outUV	= inPos.xz * 0.1f;

	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPos, 1.0f);
}