#version 330

in vec2 texCoord;
uniform sampler2D tex;
uniform vec4 color;

out vec4 outColor;

void main(void){
	float intensity = texture(tex, texCoord).r;
	//outColor = vec4(intensity, intensity, intensity, 1.0);
	outColor = intensity * color;
}
