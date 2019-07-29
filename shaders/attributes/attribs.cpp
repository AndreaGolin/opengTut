#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSourceUniform = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

const char *vertexShaderSourceAttr = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "	ourColor = aColor;\n"
    "}\0";
const char *fragmentShaderSourceAttr = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

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

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceUniform, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgramUniform = glCreateProgram();
    glAttachShader(shaderProgramUniform, vertexShader);
    glAttachShader(shaderProgramUniform, fragmentShader);
    glLinkProgram(shaderProgramUniform);
    // check for linking errors
    glGetProgramiv(shaderProgramUniform, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramUniform, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }


    // Compile the attribs shader
    //**
    //**
    // vertex attrib shader
    int vertexShaderAttr = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderAttr, 1, &vertexShaderSourceAttr, NULL);
    glCompileShader(vertexShaderAttr);
    // check for shader compile errors
    glGetShaderiv(vertexShaderAttr, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderAttr, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Frag shader attr
    int fragmentShaderAttr = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderAttr, 1, &fragmentShaderSourceAttr, NULL);
    glCompileShader(fragmentShaderAttr);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderAttr, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderAttr, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders attr
    int shaderProgramAttr = glCreateProgram();
    glAttachShader(shaderProgramAttr, vertexShaderAttr);
    glAttachShader(shaderProgramAttr, fragmentShaderAttr);
    glLinkProgram(shaderProgramAttr);
    // check for linking errors
    glGetProgramiv(shaderProgramAttr, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramAttr, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShaderAttr);
    glDeleteShader(fragmentShaderAttr);

    float vertices[] = {
        -0.9f, -0.3f, 0.0f,
        -0.3f, -0.3f, 0.0f,
        -0.6f,  0.3f, 0.0f
    };
    float verticesAttr[] = {
        // vertices 		//colors
        0.3f, -0.3f, 0.0f,	1.0f, 0.0f, 0.0f,	// bottom left
        0.9f, -0.3f, 0.0f,	0.0f, 1.0f, 0.0f,	// bottom right
        0.6f,  0.3f, 0.0f,  0.0f, 0.0f, 1.0f	// top
    };

    unsigned int VBO, VAO, VBOA, VAOA;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //
    glGenVertexArrays(1, &VAOA);
    glGenBuffers(1, &VBOA);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // Bind second VAO for attrib
    glBindVertexArray(VAOA);
    glBindBuffer(GL_ARRAY_BUFFER, VBOA);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAttr), verticesAttr, GL_STATIC_DRAW);

    // Point to position, as usual
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //Point to color!
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    // Unbind everything for the lulz
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // The Loop
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgramUniform, "ourColor");
        glUseProgram(shaderProgramUniform);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);


        glUseProgram(shaderProgramAttr);
        glBindVertexArray(VAOA);
		glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);


        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Clean Up
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