#version 450
#include "VertexAttributes.glslh"

layout (location = 0) out vec4 outColour;

uniform float time;

void main() {
	outColour = inColour;

	vec3 pos = inPos;

	pos.x += sin(time);

	gl_Position = vec4(pos, 1.0f);
}