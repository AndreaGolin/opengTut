#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * Chores class for avoid implementation etc.
 */
#include <chores/chores.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/**
 * Shader header class
 */
#include <myshaders/shader_s.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char const *argv[])
{
	// Chore class for day to day openglling
	/**
	/ Inits GLFW, GLAD, Window Object
	/*/
	Chores chore;

	GLFWwindow* window = chore.CreateWindow();
    
    /**
     * Make window our context and bind the callbacks
     */
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /**
     * Init GLAD function pointers
     */
    chore.InitGlad();

    // Generate a texture
    unsigned int texture;
    glGenTextures(1, &texture);
    // Bind it, from now on all texture calls will be redirected to this texture object
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /**
     * Enable depth testing
     */
    glEnable(GL_DEPTH_TEST);

    /**
     * Load image and generate texture
     */
    // Load an image with stb_image
    int width, height, nrChannels;
    unsigned char *data = stbi_load("../textures/container.jpg", &width, &height, &nrChannels, 0); 
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
    //release the image memory
    stbi_image_free(data);

    // Load the shader
    Shader coordsShader("/home/andrea/opengl/shaders/shaders_src/shaderCoords.vs", "/home/andrea/opengl/shaders/shaders_src/shaderTexture.fs");

    // define vertices, with added texture coords!
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    // Define and bind VAO e VBO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Point to position, as usual
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Point to color!
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // Point to textures!
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    coordsShader.use();
    unsigned int modelLoc = glGetUniformLocation(coordsShader.ID, "ModelMat");
    unsigned int viewLoc  = glGetUniformLocation(coordsShader.ID, "ViewMat");
    unsigned int projLoc  = glGetUniformLocation(coordsShader.ID, "ProjectionMat");
    unsigned int funLoc   = glGetUniformLocation(coordsShader.ID, "FunMat");

    /**
     * Create a Model matrix
     * Multiplyng the model matrix with the vertex transform the obj coords to world coords
    */
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::rotate(modelMat, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    /**
     * View Matrix
     * To change the world view, camera-like
     */
    glm::mat4 viewMat = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move
	// we move the scene to the negative z axis, which lies ahead of us to give the impression 
	// of zooming out
	viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -3.0f));

	/**
	 * Projection matrix
	 * To give our scene ortho/perspective view
	 */
	glm::mat4 projectionMat;
	projectionMat = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


	
	// The Loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        // clear
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Funny rotation
        glm::mat4 funMat = glm::mat4(1.0f);
        funMat = glm::rotate(funMat, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
       
		glBindVertexArray(VAO);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMat));
		glUniformMatrix4fv(funLoc, 1, GL_FALSE, glm::value_ptr(funMat));
        // This call will automatically bind the texture to the uniform texture of the frag shader
        glBindTexture(GL_TEXTURE_2D, texture);
        
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}