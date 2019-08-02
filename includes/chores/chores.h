#ifndef CHORES_H
#define CHORES_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Chores
{
public:

	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	GLFWwindow* window;

	Chores(){
		initGlfw();
	}
	~Chores(){}

	/**
	 * @brief      Create the window object
	 *
	 * @return     the window object
	 */
	GLFWwindow* CreateWindow(){
		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	    if (window == NULL)
	    {
	        std::cout << "Failed to create GLFW window" << std::endl;
	        glfwTerminate();
	        this->returnError();
	    }
	    return window;
	}

	// GLAD init
    // glad: load all OpenGL function pointers
    // ---------------------------------------
	void InitGlad(){
	    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	    {
	        std::cout << "Failed to initialize GLAD" << std::endl;
	        this->returnError();
	    }
	}

private:

	/**
	 * @brief      Init GLFW
	 */
	void initGlfw(){
		glfwInit();
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	/**
	 * @brief      Helper for returning error codes
	 *
	 * @return     the return error
	 */
	int returnError(){
		return -1;
	}

};
#endif