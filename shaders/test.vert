#version 330

in vec3 in_Position;
out vec2 xyPos;
//badle named shaders which will later be changed to be able to handle textures
void main(void){
		xyPos = in_Position.xy;
		gl_Position = vec4(in_Position, 1.0);
}
