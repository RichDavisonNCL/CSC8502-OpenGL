#version 450

layout (location = 0) in vec4 inColour;

out vec4 fragColour;

uniform float time;

uniform vec4 colour;

uniform bool useColour;

void main() {
	if(useColour) {
		fragColour = colour;
	}
	else {
		fragColour = inColour;
	}
}