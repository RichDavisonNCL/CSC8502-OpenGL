#version 450
uniform sampler2D albedoTex;

in Vertex {
	vec2 uv;
} IN;

out vec4 fragColour;

void main() {
	fragColour = texture ( albedoTex , IN.uv.xy );
}