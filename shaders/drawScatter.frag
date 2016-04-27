#version 330

in vec2 texCoord;
in vec2 pass_Position;
uniform sampler2D scatterTex;
out vec4 out_Color;

//will sample texture and set color from the texture
void main(void){
	float intensity = 0;
	intensity = texture(scatterTex, texCoord).r;	
	//out_Color = vec4(1,0, 0, 1);
	out_Color = vec4(0.0, intensity/5.0, 0.0, 1);
}
