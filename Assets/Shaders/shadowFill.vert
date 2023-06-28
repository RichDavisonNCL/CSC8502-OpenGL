#version 450
#include "VertexAttributes.glslh"

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPos, 1.0f);
}