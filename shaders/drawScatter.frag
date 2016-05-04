#version 330

in vec2 texCoord;
in vec2 pass_Position;
uniform sampler2D scatterTex;
out vec4 out_Color;
float offset = 1.0/600.0;

float weight = 0.0;
float sigma = 1.0;
float xpos = 0.0;
float ypos = 0.0;
float M_PI = 3.14;
float intensity = 0.0;

//will sample texture and set color from the texture
void main(void){

	//intensity = texture(scatterTex, texCoord).r;	
	//out_Color = vec4(1, 0, 0, 1);
 	float var2 = 2.0*sigma*sigma;
	float var1 = 1.0/(2.0 * M_PI * sigma*sigma);	

	 for(int i = -15; i < 15; i++){
		for(int j = -15; j < 15; j++){ 
			xpos = texCoord.x + j*offset;
			ypos = texCoord.y + i*offset;
			
			if(xpos >= 0 && xpos <= 1 && ypos >= 0 && ypos <= 1){
			intensity = intensity + var1*exp(-(pow(i, 2) + pow(j, 2))/(var2)) * texture(scatterTex, vec2(xpos, ypos)).r;
			//intensity = 1.0;
			} 

		}
	}	

	out_Color = vec4(0.0, intensity, 0.0, 1);
}
