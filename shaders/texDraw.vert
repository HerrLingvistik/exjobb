#version 330

in vec2 in_Position;
out vec2 texCoord;

void main(void){
	texCoord = in_Position.xy / 2.0 + 0.5;
	gl_Position = vec4(in_Position, 0.0, 1.0);
}
