#version 450 core

uniform sampler2D albedoTex;
uniform sampler2D bumpTex;

in Vertex {
	vec2 uv;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
} IN;

out vec4 fragColour[2];

void main() {
	mat3 TBN = mat3 ( normalize ( IN . tangent ) ,
	normalize ( IN . bitangent ) ,
	normalize ( IN . normal ));

	vec3 normal = texture2D (bumpTex , IN.uv ). rgb;
	normal = normalize ( TBN * normalize ( normal * 2.0 - 1.0 ));

	fragColour [0] = texture2D ( albedoTex , IN.uv );
	fragColour [1] = vec4 ( normal . xyz * 0.5 + 0.5 ,1.0);
}