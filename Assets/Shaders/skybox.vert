#version 450
#include "VertexAttributes.glslh"
#include "Camera.glslh"

layout(location = 0) uniform Camera cam;

layout (location = 0) out vec4 outColour;

out Vertex {
	vec3 viewDir;
} OUT;

void main() {
	vec3 pos = inPos ;

	pos.xy /= vec2 ( cam.projMatrix [0][0] , cam.projMatrix [1][1]);
	pos.z = -1.0f;

	OUT.viewDir = transpose ( mat3 ( cam.viewMatrix )) * normalize ( pos );

	gl_Position = vec4 ( inPos , 1.0); 
}