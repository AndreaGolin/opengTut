#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


/**
 * Here follow the source code for both vertex and fragment shader
 */
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec3(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{

	// glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

    // window creation
    GLFWwindow* window = glfwCreateWindow(400, 150, "my first triangle", NULL, NULL);
    if (window == NULL)
	{
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
	    return -1;
	}
	
	// context and resize redirect
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad init
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    std::cout << "Failed to initialize GLAD" << std::endl;
	    return -1;
	}

	/**
	 * Shaders
	 */

    // declare a vertex shader
    unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Bind the vertex shader object to the vertex shader source, then compile it
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for compilation errors
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
	    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /**
     * At this stage we got both the vertex and fragment shader compiled
     * We will combiene (link) them into a Shader Program we will use to render objects
     */

    // Define the Shader Program
    unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// Link the compiled shaders ito the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check linking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    /**
     * The result is a program object that we can activate by calling glUseProgram 
     * with the newly created program object as its argument: glUseProgram(shaderProgram);
     * After invoking glUserProgram every draw call will use the program
     */

    // Delete the shaders, as they are not needed anymore
    glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader); 

	/**
	 * Set first vertex, composed of three vertices
	 * This represents the three vertices of a triangle
	 * We will send this to the GPU, through the VBO buffer
	 */
	float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    // Init the VBO (Vertex Buffer Object)
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Init the VAO (Vertex Array Object)
    unsigned int VAO;
	glGenVertexArrays(1, &VAO); 
	glBindVertexArray(VAO);

	// 0. copy our vertices array in a buffer for OpenGL to use
	// bind the buffer to vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	 /**
     * glBufferData send data to a bound buffer
     * The first parameter is the type of buffer
     * The second is the size in bytes of the data we are sending
     * Third we send the acual data
     * Fouth parameter refers to how the memeory should handle the data we sent
     */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 1. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 2. use our shader program when we want to render an object
	// glUseProgram(shaderProgram);
	// 3. now draw the object 
	// someOpenGLFunctionThatDrawsOurTriangle(); 
	// 4. draw the object
	// glUseProgram(shaderProgram);
	// glBindVertexArray(VAO);
	// someOpenGLFunctionThatDrawsOurTriangle(); 


	// render loop
	while(!glfwWindowShouldClose(window))
	{
		
		processInput(window);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// rendering commands here
		// glclear all previous iteration drawing from screen (in this case, the color buffer)
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

	    glfwSwapBuffers(window);

	    glfwPollEvents();
	}

	// clean up
	glfwTerminate();
    return 0;
}

/**
 * @brief      Called everytime the window is resized
 *
 * @param      GLFWwindow window  The window
 * @param[in]  width   The width
 * @param[in]  height  The height
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    /**
	 * The first two parameters of glViewport set the location of the lower left corner of the window. 
	 * The third and fourth parameter set the width and height of the rendering window in pixels, which we set equal to GLFW's window size. 
	 */
	glViewport(0, 0, width, height);
} 

/**
 * @brief      Process user input
 *
 * @param      window  The window
 */
void processInput(GLFWwindow *window)
{
	/**
	 * glfwGetKey is the function used to manage key actions
	 */
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){

    	std::cout << "Esc key pressed, exiting\n";

    	// Set the rendering loop to close at next generation
        glfwSetWindowShouldClose(window, true);
    }
}