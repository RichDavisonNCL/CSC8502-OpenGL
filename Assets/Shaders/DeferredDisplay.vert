#version 450 core
#include "VertexAttributes.glslh"

out Vertex {
	vec2 uv;
} OUT ;

void main ( void ) {
	gl_Position = vec4 ( inPos , 1.0);
	OUT.uv = inUV;
}