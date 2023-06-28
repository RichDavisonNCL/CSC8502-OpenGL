#version 450
#include "VertexAttributes.glslh"
#include "Camera.glslh"

layout(location = 0) uniform Camera cam; 

uniform mat4 modelMatrix;
uniform mat4 shadowMatrix;

out Vertex {
	vec2 uv;
	vec3 normal;
	vec3 worldPos;
	vec4 shadowProjection;
} OUT;

void main() {
	OUT.uv = inUV;
	TransformNormal(modelMatrix, OUT.normal);

	vec4 worldPos = modelMatrix * vec4(inPos, 1.0f);
	OUT.worldPos = worldPos.xyz;

	gl_Position = cam.projMatrix * cam.viewMatrix * worldPos;

	OUT.shadowProjection = shadowMatrix * worldPos;
}