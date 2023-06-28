#version 450 core
#include "VertexAttributes.glslh"
#include "light.glslh"
#include "Camera.glslh"

layout(location = 0) uniform Camera cam;

layout(std430, binding = 0) buffer Lights
{
	Light lights[];
};

out Vertex {
	flat int lightID;
} OUT;

void main ( void ) {
	int lightID = gl_InstanceID;

	Light l = lights[lightID];

	vec3 scale = vec3 ( l.radius, l.radius, l.radius );

	vec3 worldPos = ( inPos * scale ) + l.position ;

	gl_Position = ( cam.projMatrix * cam.viewMatrix ) * vec4 ( worldPos , 1.0);

	OUT.lightID = lightID;
}