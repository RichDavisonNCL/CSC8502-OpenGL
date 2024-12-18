#version 450
#include "VertexAttributes.glslh"
#include "Camera.glslh"

layout(location = 0) uniform Camera cam;

layout (location = 0) out vec4 outColour;

uniform mat4 modelMatrix;

out Vertex {
	vec2 uv;
	vec3 tangent;
	vec3 bitangent;
	vec3 normal;
	vec3 worldPos;
} OUT;

void main() {
	vec4 worldPos	= modelMatrix * vec4(inPos, 1.0f);

	OUT.worldPos	= worldPos.xyz;
	OUT.uv			= inUV;

	CreateTBNVectors(modelMatrix, OUT.tangent, OUT.bitangent, OUT.normal);

	gl_Position = cam.projMatrix * cam.viewMatrix * worldPos;
}