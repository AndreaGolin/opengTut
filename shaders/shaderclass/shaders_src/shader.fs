#version 330 core
out vec4 FragColor;
uniform float ourColor;
void main(){
	FragColor = vec4(0.3f,ourColor,0.1f,1.0f);
}