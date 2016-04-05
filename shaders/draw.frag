#version 330

in vec2 texCoord;
uniform sampler2D parallelTex;
out vec4 out_Color;
 
uniform float maxValue;

//will sample texture and set color from the texture
void main(void){
	float intensity = 0;
	float red = 0;
	intensity = texture(parallelTex, texCoord).r;	
		
	float scale = 1.0;
	out_Color = vec4(log(log(intensity+1)/log(10) +1)/log(10), log(log(intensity+1)/log(10) +1)/log(10), log(log(intensity+1)/log(10) +1)/log(10), 1);
	//out_Color = vec4(0.5, 0, 0, 1);
}
