#version 450 core
#include "Light.glslh"
#include "Camera.glslh"

layout(location = 0) uniform Camera cam;
layout(location = 5) uniform Light light;

uniform sampler2D albedoTex;
uniform sampler2D shadowTex;

in Vertex {
	vec2 uv;
	vec3 normal;
	vec3 worldPos;
	vec4 shadowProjection;
} IN;

out vec4 fragColour;

void main() {
	vec3 diffuse	= vec3(0);
	vec3 specular	= vec3(0);

	float roughness = 50.0f;

	BlinnPhong(light, IN.normal, cam.position, IN.worldPos, roughness, diffuse, specular);

	float shadow = CalculateShadow(IN.shadowProjection, shadowTex);

	vec4 albedo = texture ( albedoTex , IN.uv.xy );
	vec3 finalColour = ((diffuse * albedo.xyz) + specular) * shadow;

	fragColour = vec4(finalColour, albedo.w);
}