#version 330

in vec2 in_Position;
uniform float height;
//simple shader setting position for the coordinates
void main(void){
		gl_Position = vec4(in_Position.x, in_Position.y*2.0/height - 1.0, 0, 1.0);
		//gl_Position = vec4(in_Position, 0, 1.0);
}
