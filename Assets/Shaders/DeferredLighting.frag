# version 450 core
#include "Camera.glslh"
#include "light.glslh"

uniform sampler2D depthTex;
uniform sampler2D normTex;

uniform vec2 pixelSize; // reciprocal of resolution

uniform mat4 inverseProjView;

layout(location = 0) uniform Camera cam;

layout(std430, binding = 0) buffer Lights
{
	Light lights[];
};

in Vertex {
	flat int lightID;
} IN;

out vec4 diffuseOutput;
out vec4 specularOutput;

void main ( void ) {
	vec2 texCoord = vec2 ( gl_FragCoord . xy * pixelSize );
	float depth = texture ( depthTex , texCoord . xy ). r ;
	vec3 ndcPos = vec3 ( texCoord , depth ) * 2.0 - 1.0;
	vec4 invClipPos = inverseProjView * vec4 ( ndcPos , 1.0);
	vec3 worldPos = invClipPos . xyz / invClipPos . w ;

	Light l = lights[IN.lightID];

	float dist = length ( l.position - worldPos );

	if(dist > l.radius) {
		discard;
	}

	vec3 normal		= normalize ( texture ( normTex , texCoord . xy ). xyz *2.0 -1.0);
	vec3 incident	= normalize ( l.position - worldPos );
	vec3 viewDir	= normalize ( cam.position - worldPos );
	vec3 halfDir	= normalize ( incident + viewDir );

	vec3 diffuse	= vec3(0);
	vec3 specular	= vec3(0);

	float roughness = 50.0f;

	AttenuatedBlinnPhong(l, normal, cam.position, worldPos, roughness, diffuse, specular);

	diffuseOutput	= vec4 ( diffuse , 1.0);
	specularOutput	= vec4 ( specular , 1.0);
}
