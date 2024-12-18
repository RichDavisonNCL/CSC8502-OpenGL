#version 450
#include "ObjectStruct.h"

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inColour;
layout (location = 2) in vec2 inUV;

layout (location = 0) out vec4 outColour;
layout (location = 1) out vec2 outUV;

uniform mat4 viewMatrix		= mat4(1.0f);
uniform mat4 projMatrix		= mat4(1.0f);

//binding, not location
layout(std140, binding = 0) buffer objectData {
	ObjectStruct objects[];
};

void main() {
	ObjectStruct object = objects[gl_InstanceID];

	outColour = object.colour;

	vec3 vertPos = inPos * object.size;
	vertPos += object.position;

	gl_Position = projMatrix * viewMatrix * vec4(vertPos, 1.0f);
}