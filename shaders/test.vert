#version 330

in vec3 in_Position;
out vec2 xyPos;

void main(void){
		xyPos = in_Position.xy;
		gl_Position = vec4(in_Position, 1.0);
}
