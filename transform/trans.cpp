#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <myshaders/shader_s.h>

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, char const *argv[])
{
	// Create glwf window, set context
	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD init
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

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
    Shader ourShader("/home/andrea/opengl/shaders/shaders_src/shaderTexture.vs", "/home/andrea/opengl/shaders/shaders_src/shaderTexture.fs");

    // define vertices, with added texture coords!
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // Define and bind VAO e VBO
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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

    //use shader program
    // ourShader.use();

    //std::cout << "Translating a vec3(1,0,0) to vec3(1,1,1)";
    // Decalaring the vec3(x,y,z) as vec4(x,y,z,w) to be able to operate with 4x4 matrix.
    // Just keep w = 1
    //glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    // Declare the identity matrix, so we can work with a neutral but initialized 4x4 matrix
    //glm::mat4 trans =  glm::mat4(1.0f);
    // Create the translate transform matrix, transalting for a vec3(1,1,1)
    //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    // Translate the vector, multiply the vec for the trans matrix
    // BEWARE! Products of matrices are NOT commutative that is A⋅B≠B⋅A.
    //vec = trans * vec;
    // Printout the vec
    //std::cout << "\nResulting vec: \n";
    //std::cout << "X: " << vec.x << "\n" << "Y: " <<vec.y << "\n" << "Z: " <<vec.z << std::endl;

    //scale and rotate the container object
    // Prepare the transform matrix
    // BEWARE! The FIRST transform is the one at the far RIGHT. So to scale first, rotate last, put rotate * scale.
    // Init the indentity mat
    // It has been transferred inside the loop, for continuous rotation
    //glm::mat4 trans =  glm::mat4(1.0f);
    // Rotate 90deg on the z axis. The vec3(0,0,1) will ensure we are rotating the z axis
    //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    // Scale to 0.5
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    // Now we must feed our Vertex Shader with the transform matrix, and multiply the vertex. 
    // We add a mat4 unifor to the VS and then multiply the position for the mat
    // See shaders/shaders_src/transform.vs
    
    // Create the new trans shader program
    Shader transShad("/home/andrea/opengl/shaders/shaders_src/transform.vs", "/home/andrea/opengl/shaders/shaders_src/shaderTexture.fs");
    // Activate the shader
    transShad.use();
    // Feed it with the trans matrix
	unsigned int transformLoc = glGetUniformLocation(transShad.ID, "TransMat");
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

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
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 trans = glm::mat4(1.0f);
		// trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));

        float timeValue = glfwGetTime();
        float sinTine = (sin(timeValue) / 2.0f) + 0.5f;
        // std::cout << sinTine << "\n";

		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(sinTine, sinTine, sinTine));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        // This call will automatically bind the texture to the uniform texture of the frag shader
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

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