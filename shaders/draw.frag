#version 330

in vec2 texCoord;
uniform usampler2D parallelTex;
out vec4 out_Color;
//will sample texture and set color from the texture
void main(void){
	float intensity = 0;
	float red = 0;
	intensity = texture(parallelTex, texCoord).r;	
	//if(intensity <= 1)
		//intensity = 0;
	
	//out_Color = vec4(red,intensity,0, 1);
	out_Color = vec4(intensity, intensity, intensity, 1);
}
