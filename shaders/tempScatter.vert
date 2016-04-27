#version 330

in float in_PositionX;
in float in_PositionY;
//in vec2 in_Position;

//simple shader setting position for the coordinates
void main(void){
	//gl_Position = vec4(0, 0, 0, 1.0);
	gl_Position = vec4(in_PositionX, in_PositionY, 0, 1.0);
	//gl_PointSize = 5.0;
	//gl_Position = vec4(in_Position.x, in_Position.y, 0.0, 1.0);
	//gl_Position = vec4(-1.0	, 1.0, 0, 1.0);
}
