#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

// built-in data-type for texture objects called a sampler
uniform sampler2D ourTexture;

void main(){

	/**
	 * built-in texture function that takes as its first argument a texture sampler 
	 * and as its second argument the corresponding texture coordinate.
	 */
	FragColor = texture(ourTexture, TexCoord); 
}