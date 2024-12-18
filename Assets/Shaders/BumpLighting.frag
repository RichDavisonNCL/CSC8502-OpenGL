#version 450 core
#include "Camera.glslh"
#include "Light.glslh"

layout(location = 0) uniform Camera cam;

layout(std140, binding = 1) uniform LightData {
	Light light;
};

uniform sampler2D albedoTex;
uniform sampler2D bumpTex;

in Vertex {
	vec2 uv;
	vec3 tangent;
	vec3 bitangent;
	vec3 normal;
	vec3 worldPos;
}IN ;

out vec4 fragColour;

void main() {
	vec3 diffuse	= vec3(0);
	vec3 specular	= vec3(0);

	vec4 albedo = texture ( albedoTex , IN.uv.xy );
	vec3 normal = texture ( bumpTex , IN.uv.xy ).xyz;

	mat3 tbn = mat3(normalize(IN.tangent), normalize(IN.bitangent), normalize(IN.normal));

	normal = normalize(tbn * normal);

	float roughness = 50.0f;

	BlinnPhong(light, normal, cam.position, IN.worldPos, roughness, diffuse, specular);

	vec3 finalColour = ((diffuse * albedo.xyz) + specular);

	fragColour = vec4(finalColour, albedo.w);
}