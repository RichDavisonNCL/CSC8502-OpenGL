#version 450

uniform samplerCube cubeTex ;

in Vertex {
	vec3 viewDir ;
} IN ;

out vec4 fragColour ;

void main() {
	fragColour = texture ( cubeTex , normalize ( IN . viewDir ));
}