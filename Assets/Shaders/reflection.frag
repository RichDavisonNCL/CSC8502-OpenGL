#version 450 core
#include "Camera.glslh"

layout(location = 0) uniform Camera cam;

uniform samplerCube cubeTex ;

in Vertex {
	vec3 normal;
	vec3 worldPos;
}IN ;

out vec4 fragColour ;

void main ( void ) {
	vec3 viewDir = normalize ( cam.position - IN.worldPos );

	vec3 reflectDir = reflect ( - viewDir , normalize ( IN . normal ));

	fragColour = texture ( cubeTex , reflectDir );
}