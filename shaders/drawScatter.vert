#version 330

in vec2 in_Position;
out vec2 texCoord;
out vec2 pass_Position;
//badle named shaders which will later be changed to be able to handle textures
void main(void){
	pass_Position = in_Position;
	texCoord = in_Position.xy / 2.0 + 0.5;
	gl_Position = vec4(in_Position, 0.0, 1.0);
	//gl_Position = vec4(0.0,0.0, 0.0, 1.0);
}
