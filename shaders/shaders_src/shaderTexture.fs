#version 330 core

out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

// built-in data-type for texture objects called a sampler
uniform sampler2D ourTexture;
uniform float ourVal0;
uniform float ourVal1;

void main(){

	/**
	 * built-in texture function that takes as its first argument a texture sampler 
	 * and as its second argument the corresponding texture coordinate.
	 */
	// FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0); 
	FragColor = texture(ourTexture, TexCoord) * vec4(ourVal0,ourVal1,0.1f, 1.0); 
}