#version 330

in vec2 texCoord;
uniform sampler2D parallelTex;
uniform sampler2D parallelTex2;
out vec4 out_Color;

float offsetX = 1.0/1299.0;
float offsetY = 1.0/620.0;
 
float weight = 0.0;
float sigma = 1.0;
float xpos = 0.0;
float ypos = 0.0;
float M_PI = 3.14;
float intensity = 0.0;
float intensity2 = 0.0;


uniform float maxValue;

//will sample texture and set color from the texture
void main(void){

	float red = 0;
	
	float var2 = 2.0*sigma*sigma;
	float var1 = 1.0/(2.0 * M_PI * sigma*sigma);	
		
  for(int i = -15; i < 15; i++){
		for(int j = -15; j < 15; j++){ 
			xpos = texCoord.x + j*offsetX;
			ypos = texCoord.y + i*offsetY;
			
			if(xpos >= 0 && xpos <= 1 && ypos >= 0 && ypos <= 1){
			intensity = intensity + var1*exp(-(pow(i, 2) + pow(j, 2))/(var2)) * texture(parallelTex, vec2(xpos, ypos)).r;
			} 

		}
	}


	
		for(int i = -15; i < 15; i++){
		for(int j = -15; j < 15; j++){ 
			xpos = texCoord.x + j*offsetX;
			ypos = texCoord.y + i*offsetY;
			
			if(xpos >= 0 && xpos <= 1 && ypos >= 0 && ypos <= 1){
				intensity2 = intensity2 + var1*exp(-(pow(i, 2) + pow(j, 2))/(var2)) * texture(parallelTex2, vec2(xpos, ypos)).r;
			} 

		}
	}
	

	out_Color = vec4(log(log(intensity+1)/log(10) +1)/log(10), log(log(intensity2+1)/log(10) +1)/log(10), 0, 1);

	//out_Color = vec4(log(log(intensity+1)/log(10) +1)/log(10), log(log(intensity+1)/log(10) +1)/log(10), log(log(intensity+1)/log(10) +1)/log(10), 1);
	
}
	
