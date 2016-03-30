#version 330

in vec2 texCoord;
uniform isampler2D parallelTex;
out vec4 out_Color;
//will sample texture and set color from the texture
void main(void){
	float intensity = 0;
	if(texture(parallelTex, texCoord).r > 300)
		intensity = 1;
	out_Color = vec4(intensity, intensity, intensity, 1);
}
