#version 330

//out uint out_Color;
out vec4 out_Color;

uniform vec4 color;

void main(void){
	out_Color = vec4(color);
}
