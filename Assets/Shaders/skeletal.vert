#version 450
#include "VertexAttributes.glslh"
#include "Camera.glslh"

layout(location = 0) uniform Camera cam;
uniform mat4 modelMatrix;

layout(std430, binding = 0) buffer Joints
{
	mat4 joints[];
};

void main() {
   mat4 skinMat = (joints[inJointIndices[0]] * inJointWeights[0]) +
				  (joints[inJointIndices[1]] * inJointWeights[1]) +
				  (joints[inJointIndices[2]] * inJointWeights[2]) +
				  (joints[inJointIndices[3]] * inJointWeights[3]);

	gl_Position = ( cam.projMatrix * cam.viewMatrix )  * modelMatrix * skinMat * vec4(inPos, 1.0f);
}