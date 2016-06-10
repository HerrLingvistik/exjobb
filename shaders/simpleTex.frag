#version 330

in vec2 texCoord;
uniform sampler2D tex;

out vec4 outColor;

void main(void){
	outColor = texture(tex, texCoord);
}
