#version 330

in vec2 texCoord;
uniform sampler2D tex;
uniform vec4 color;
uniform vec4 color2;
uniform int backgroundcolor;	

out vec4 outColor;

void main(void){
	float intensity = texture(tex, texCoord).r;
	float intensity2 = texture(tex, texCoord).g;
	
	if(backgroundcolor == 0){	
		outColor = intensity * color + intensity2 * color2;
		float overshoot = max(outColor.x, outColor.z);
		//if(overshoot != 0 )
			//outColor.xyz = outColor.xyz/5.0;
	}
	else if(backgroundcolor == 1){
		//outColor = vec4(1,1,1,1) - vec4(1.0-color.x,1.0-color.y,1.0-color.z,0.0)*intensity + (vec4(1,1,1,1) - vec4(1.0-color2.x,1.0-color2.y,1.0-color2.z,0.0)*intensity2);
		//vec4(1.0,1.0-intensity,1.0-intensity,1.0);
		vec4 col1 = intensity * color;
		vec4 col2 = intensity2 * color2;
		outColor = vec4(1.0,1.0,1.0,1.0) + col1+col2;
		float overshoot = max(outColor.x, outColor.z);
		outColor.xyz = outColor.xyz/overshoot;
	}
}
