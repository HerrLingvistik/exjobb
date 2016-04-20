#version 330

in vec2 in_Position;

//simple shader setting position for the coordinates
void main(void){
		gl_Position = vec4(in_Position, 0, 1.0);
}
