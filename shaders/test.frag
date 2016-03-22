#version 330

in vec2 xyPos;
out vec4 out_Color;
//will sample texture and set color from the texture
void main(void){
	out_Color = vec4(abs(xyPos.x),abs(xyPos.y),0,1);
}
