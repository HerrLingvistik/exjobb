#version 330

//out uint out_Color;
out vec4 out_Color;

uniform vec3 color;

void main(void){
	//out_Color = uint(1);
	out_Color = vec4(color, 1);
}