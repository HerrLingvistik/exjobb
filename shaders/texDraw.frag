#version 330

in vec2 texCoord;
uniform sampler2D tex;
uniform vec4 color;
uniform int backgroundcolor;

//#define BLACK = 0;
//#define WHITE = 1;	

out vec4 outColor;

void main(void){
	float intensity = texture(tex, texCoord).r;
	//outColor = mix(vec4(1.0,1.0,1.0,1.0), color, intensity);
	if(backgroundcolor == 0){	
		outColor = intensity * color;
	}
	else if(backgroundcolor == 1){
		outColor = vec4(1,1,1,1) - vec4(1.0-color.x,1.0-color.y,1.0-color.z,1.0)*intensity;//vec4(1.0,1.0-intensity,1.0-intensity,1.0);
	}
}
