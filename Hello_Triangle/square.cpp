#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//prototype for callback function for when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//no processing, this shader just forwards the input to the output
//we set the input location to 0 and then return a vec4 with the input and 1.0 in the 4th element
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    //
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Register the framebuffer size callback so  GLFW knows to call it on every window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }







    //VERTEX SHADER STUFF   
    //shader is complied dynamically at run time from source code
    //create a shader object refrenced by an ID, we specify that this is a vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //Attach the shader source code to the shader object and then complie them
    //glShaderSource(shader object , number of strings passed as source code, actual source code of shader,NULL);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //Check if vertex shader complied successfully
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
        infoLog << std::endl;
    }


    //FRAGMENT SHADER STUFF
    //fragment shader calculates the color ouput of the pixels
    //Processes is similar to vertex shader only glCreateShader has Fragment shader set as the type
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    //create shader program object, set it to an ID refrence
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //Attatch the vertex and fragment shader to the shader program, then link them
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    //every shader and rendering call after this will use shaderProgram
    glUseProgram(shaderProgram);

    //delete the shader objects now that they're already linked into the program, no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // float vertices[] = {
    // -0.5f, -0.5f, 0.0f,
    // 0.5f, -0.5f, 0.0f,
    // 0.0f, 0.5f, 0.0f
    // };
    float vertices[] = {
    0.5f, 0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f // top left
    };
    unsigned int indices[] = { // note that we start from 0!
    0, 1, 3, // first triangle
    1, 2, 3 // second triangle
    };


    //vertex buffer object has a unique ID we can generate using glGenBuffers
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    //Then we bind VBO to the GL_ARRAY_BUFFER target
    //Now any calls made on the GAB target will be used to configure the current bound buffer VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Copies vertex data,vertices, into the buffers memory
    //glBufferData copies user-defined data into the current bound buffer
    //(type of buffer to copy data into, size of data(bytes), data to sent, how the graphics card should manage the data);
    /*
        • GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
        • GL_STATIC_DRAW: the data is set only once and used many times.
        • GL_DYNAMIC_DRAW: the data is changed a lot and used many times.
    */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //Tell openGL how to inturprut vertex data and shader attributes
    //arg1: specifies vertex attribute to be configured,in vertex shader source we set this to 0 so we pass 0 here
    //arg2: size of the vertex attribute(vec3 so 3 values)
    //arg3: data type, in this case GL_FLOAT(vecs in glsl are made up of floats)
    //arg4: Asks if data should be normalized(relevant if we're using integers that need to be normalized)
    //arg5: stride, the space between consecutive vertex attributes. we're using vec3 which are made up of three floats
    // so the size of each of these vectors is 3* float size. Because the array is tightly packed(no space between values)
    //we could have set this to 0 and let OpenGL determine the stride(only works if values are tightly packed)
    //arg6: offset of where the position of deta begins in the buffer, in this case the position is at the start of the array so we set this to 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);


    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 

    


    //Render loop
    while(!glfwWindowShouldClose(window))
    {

        processInput(window);


        //clear the buffer with this color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
       // glDrawArrays(GL_TRIANGLES, 0, 3);     
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //Checks if any events are triggerd, updates the window state, and calls corresponding functions(that can be registered via callback methods)
        glfwPollEvents();

        //swap the color buffer (double buffer used to prevent artifacts)
        glfwSwapBuffers(window);
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);


    glfwTerminate();

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}