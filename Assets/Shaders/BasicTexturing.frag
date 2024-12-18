#version 450 core

layout (location = 0) in vec4 inColour;
layout (location = 1) in vec2 inUV;

uniform sampler2D tex;

out vec4 fragColour;

void main() {
	fragColour = texture(tex, inUV) * inColour;
}