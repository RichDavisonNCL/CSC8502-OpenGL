#version 450
//#include "VertexAttributes.glslh"

layout (location = 0) in vec3 inPos;
layout (location = 2) in vec2 inUV;
layout (location = 5) in vec4	inJointWeights;
layout (location = 6) in ivec4	inJointIndices;

uniform mat4 modelMatrix	= mat4(1.0f);
uniform mat4 viewMatrix		= mat4(1.0f);
uniform mat4 projMatrix		= mat4(1.0f);

uniform vec4 objectColour	= vec4(1.0f, 1.0f, 1.0f, 1.0f);

layout (location = 0) out vec4 outColour;
layout (location = 1) out vec2 outUV;

/*
layout(std430, binding = 0) buffer Joints
{
	mat4 joints[];
};
*/

uniform mat4 joints[128];

void main() {
	vec4 localPos = vec4(inPos, 1.0f);

	vec4 skelPos =	joints[inJointIndices[0]] * localPos * inJointWeights[0] +
					joints[inJointIndices[1]] * localPos * inJointWeights[1] +
					joints[inJointIndices[2]] * localPos * inJointWeights[2] +
					joints[inJointIndices[3]] * localPos * inJointWeights[3];	

	skelPos.w = 1.0f; //Just to be sure!

	gl_Position = ( projMatrix * viewMatrix )  * modelMatrix * skelPos;
	outColour = objectColour;
	outUV = inUV;
}