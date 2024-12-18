#version 450
#include "VertexAttributes.glslh"
#include "Camera.glslh"

layout (location = 0) out vec4 outColour;
layout (location = 1) out vec2 outUV;

layout(location = 0) uniform Camera cam;

uniform  sampler2D heightTex;

uniform float maxHeight = 1.0f;
uniform float texScale  = 1.0f;
uniform vec2  gridScale = vec2(1,1);

void main() {
	outColour	= vec4(1,1,1,1);
	

	vec4 vertexPos = vec4(inPos, 1.0f);

	vertexPos.y = texture(heightTex, vertexPos.xz / gridScale).x * maxHeight;

	outUV		= vertexPos.xz / texScale;

	gl_Position = cam.projMatrix * cam.viewMatrix * vertexPos;
}