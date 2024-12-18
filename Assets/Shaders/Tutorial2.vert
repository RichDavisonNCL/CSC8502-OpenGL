#version 450 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inColour;

layout (location = 0) out vec4 outColour;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform vec4 objectColour;

void main() {
	outColour	= objectColour;

	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPos, 1.0f);
}