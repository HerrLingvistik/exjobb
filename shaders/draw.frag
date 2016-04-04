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
	//if(intensity < 2)
		//intensity = 0;
	//intensity = normalize(intensity)
	//out_Color = vec4(red,intensity,0, 1);
	out_Color = vec4(intensity/maxValue, 0, 0, 0);
}
