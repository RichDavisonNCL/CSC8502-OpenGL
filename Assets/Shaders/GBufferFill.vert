#version 450 core
#include "VertexAttributes.glslh"
#include "Camera.glslh"

layout(location = 0) uniform Camera cam; 

uniform mat4 modelMatrix;

out Vertex {
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
} OUT;

void main() {
	OUT.uv = inUV;
	CreateTBNVectors(modelMatrix, OUT.tangent, OUT.bitangent, OUT.normal);

	vec4 worldPos = modelMatrix * vec4(inPos, 1.0f);
	gl_Position = cam.projMatrix * cam.viewMatrix * worldPos;
}