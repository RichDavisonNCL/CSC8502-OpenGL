#version 450
#include "VertexAttributes.glslh"
#include "Camera.glslh"

layout(location = 0) uniform Camera cam;

uniform mat4 modelMatrix;

out Vertex {
	vec2 uv;
} OUT;

void main() {
	OUT.uv = inUV;
	gl_Position = cam.projMatrix * cam.viewMatrix * modelMatrix * vec4(inPos, 1.0f);
}