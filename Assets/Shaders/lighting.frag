#version 450 core
#include "Camera.glslh"
#include "Light.glslh"

layout(location = 0) uniform Camera cam;

layout(std140, binding = 1) uniform LightData {
	Light light;
};

uniform sampler2D albedoTex;

in Vertex {
	vec2 uv;
	vec3 normal;
	vec3 worldPos;
}IN ;

out vec4 fragColour;

void main() {
	vec3 diffuse	= vec3(0);
	vec3 specular	= vec3(0);

	float roughness = 50.0f;

	BlinnPhong(light, IN.normal, cam.position, IN.worldPos, roughness, diffuse, specular);

	vec4 albedo = texture ( albedoTex , IN.uv.xy );
	vec3 finalColour = ((diffuse * albedo.xyz) + specular);

	fragColour = vec4(finalColour, albedo.w);

	fragColour.rgb = IN.normal;
}