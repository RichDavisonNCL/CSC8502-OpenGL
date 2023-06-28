#version 450

layout (location = 0) in vec4 inColour;

out vec4 fragColour;

void main() {
	fragColour = inColour;
}