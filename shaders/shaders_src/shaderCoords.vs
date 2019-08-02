#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjectionMat;

out vec3 ourColor;
out vec2 TexCoord;

void main(){

	// Multiply the position for the trans mat
	gl_Position = ProjectionMat * ViewMat * ModelMat * vec4(aPos, 1.0f);

	ourColor = aColor;
	TexCoord = aTexCoord;
}