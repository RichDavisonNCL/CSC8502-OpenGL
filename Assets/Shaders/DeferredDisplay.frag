#version 450 core

uniform sampler2D albedoTex ;
uniform sampler2D diffuseLight ;
uniform sampler2D specularLight ;

in Vertex {
	vec2 uv;
} IN ;

 out vec4 fragColour ;

 void main ( void ) {
	 vec3 albedo = texture ( albedoTex , IN . uv ). xyz ;
	 vec3 diffuse = texture ( diffuseLight , IN . uv ). xyz ;
	 vec3 specular = texture ( specularLight , IN . uv ). xyz ;

	 fragColour . xyz = albedo * 0.025; // ambient
	 fragColour . xyz += albedo * diffuse ; // lambert
	 fragColour . xyz += specular ; // Specular
	 fragColour . a = 1.0;

	 vec3 tonemapped = fragColour.xyz / (fragColour.xyz + vec3(1,1,1));

	 fragColour.xyz = tonemapped;
 }