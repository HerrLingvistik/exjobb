#version 330

in float in_PositionX;
in float in_PositionY;

//simple shader setting position for the coordinates
void main(void){
	gl_Position = vec4(in_PositionX, in_PositionY, 0, 1.0);
	//gl_PointSize = 5.0;
}
