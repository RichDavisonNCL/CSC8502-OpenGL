# version 450 core
#include "light.glslh"
#include "Camera.glslh"

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
	float atten = 1.0 - clamp ( dist / l.radius , 0.0 , 1.0);

	if( atten == 0.0) {
		discard;
	}

	vec3 normal		= normalize ( texture ( normTex , texCoord . xy ). xyz *2.0 -1.0);
	vec3 incident	= normalize ( l.position - worldPos );
	vec3 viewDir	= normalize ( cam.position - worldPos );
	vec3 halfDir	= normalize ( incident + viewDir );

	float lambert	= clamp ( dot ( incident , normal ) ,0.0 ,1.0);
	float rFactor	= clamp ( dot ( halfDir , normal ) ,0.0 ,1.0);
	float specFactor = clamp ( dot ( halfDir , normal ) ,0.0 ,1.0);
	specFactor		= pow ( specFactor , 60.0 );
	vec3 attenuated = l.intensity.xyz * atten ;
	diffuseOutput	= vec4 ( attenuated * lambert , 1.0);
	specularOutput	= vec4 ( attenuated * specFactor * 0.33 , 1.0);
}
