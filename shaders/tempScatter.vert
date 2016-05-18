#version 330

in float in_PositionX;
in float in_PositionY;

//simple shader setting position for the coordinates
void main(void){
	gl_PointSize = 5.0;
	gl_Position = vec4((in_PositionX*2.0/600-1.0)*0.9, ((in_PositionY*2.0/600.0)-1.0)*0.9, 0, 1.0);

}
