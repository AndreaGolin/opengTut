#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
	/**
	 * @brief      Init glfw
	 */
    glfwInit();

    /**
     * Settings for the next create window call
     */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /**
     * Create the window
     */
    GLFWwindow* window = glfwCreateWindow(400, 150, "my first opengl window", NULL, NULL);
    if (window == NULL)
	{
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
	    return -1;
	}
	/**
	 * @brief      Set the context to the window
	 *
	 * @param[in]  GLFWwindow window
	 */
	glfwMakeContextCurrent(window);

	/**
	 * @brief      Callback for everytime the window is resized
	 *
	 * @param[in]  GLFWwindow
	 * @param[in]  
	 */
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	/**
	 * GLAD init
	 * We pass GLAD the function to load the adress of the OpenGL function pointers which is OS-specific. 
	 * GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for.
	 */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    std::cout << "Failed to initialize GLAD" << std::endl;
	    return -1;
	}

	/**
	 * Start the render loop
	 */
	while(!glfwWindowShouldClose(window))
	{
		/**
		 * @brief      Trigger the event polls
		 *
		 * @param[in]  GLFWwindow window
		 */
		processInput(window);

		// rendering commands here
		// glclear all previous iteration drawing from screen (in this case, the color buffer)
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/**
		 * @brief      Dump the color buffer inside the window
		 *
		 * @param[in]  GLFWwindow window
		 */
	    glfwSwapBuffers(window);

	    /**
	     * @brief      polls for event (like keyboard or mouse call)
	     * 				and change window state, calls corresponding methods via callback
	     */
	    glfwPollEvents();    
	}

	/**
	 * @brief      Clean up glfw and its resources
	 */
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
    // switch(glfwGetKey(window, GLFW_KEY_ESCAPE)){
    // 	case GLFW_PRESS:
    // 		std::cout << "Esc key pressed, exiting\n";
    // 		glfwSetWindowShouldClose(window, true);
    // 		break;

    // }

	    
}