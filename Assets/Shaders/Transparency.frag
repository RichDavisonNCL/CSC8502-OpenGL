#version 450

layout (location = 0) in vec4 inColour;
layout (location = 1) in vec2 inUV;

uniform sampler2D albedoTex;

out vec4 fragColour;

void main() {
	fragColour = texture(albedoTex, inUV) * inColour;
}