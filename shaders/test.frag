#version 330

in vec2 xyPos;
out vec4 out_Color;

void main(void){
	out_Color = vec4(abs(xyPos.x),abs(xyPos.y),0,1);
}
