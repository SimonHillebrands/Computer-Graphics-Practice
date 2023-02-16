#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("bu.vs", "bu.fs");


    float vertices[] = {
        -0.25f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.25f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.25f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.25f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.25f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.25f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.25f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.25f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.25f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.25f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.25f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.25f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.25f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.25f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.25f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.25f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.25f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.25f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.25f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.25f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.25f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.25f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.25f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.25f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.25f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.25f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.25f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.25f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.25f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.25f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.25f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.25f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.25f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.25f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.25f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.25f,  0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f, -0.5f, -0.25f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.25f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.25f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.25f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.25f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.25f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.25f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.25f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.25f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.25f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.25f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.25f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.25f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.25f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.25f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.25f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.25f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.25f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.25f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.25f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.25f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.25f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.25f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.25f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.25f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.25f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.25f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.25f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.25f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.25f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.25f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.25f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.25f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.25f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.25f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.25f,  0.0f, 1.0f,


        -0.25f, -0.75f, -0.25,  0.0f, 0.0f,
         0.25f, -0.75f, -0.25,  1.0f, 0.0f,
         0.25f,  0.75f, -0.25,  1.0f, 1.0f,
         0.25f,  0.75f, -0.25,  1.0f, 1.0f,
        -0.25f,  0.75f, -0.25,  0.0f, 1.0f,
        -0.25f, -0.75f, -0.25,  0.0f, 0.0f,
        -0.25f, -0.75f,  0.25,  0.0f, 0.0f,
         0.25f, -0.75f,  0.25,  1.0f, 0.0f,
         0.25f,  0.75f,  0.25,  1.0f, 1.0f,
         0.25f,  0.75f,  0.25,  1.0f, 1.0f,
        -0.25f,  0.75f,  0.25,  0.0f, 1.0f,
        -0.25f, -0.75f,  0.25,  0.0f, 0.0f,
        -0.25f,  0.75f,  0.25,  1.0f, 0.0f,
        -0.25f,  0.75f, -0.25,  1.0f, 1.0f,
        -0.25f, -0.75f, -0.25,  0.0f, 1.0f,
        -0.25f, -0.75f, -0.25,  0.0f, 1.0f,
        -0.25f, -0.75f,  0.25,  0.0f, 0.0f,
        -0.25f,  0.75f,  0.25,  1.0f, 0.0f,

         0.25f,  0.75f,  0.25,  1.0f, 0.0f,
         0.25f,  0.75f, -0.25,  1.0f, 1.0f,
         0.25f, -0.75f, -0.25,  0.0f, 1.0f,
         0.25f, -0.75f, -0.25,  0.0f, 1.0f,
         0.25f, -0.75f,  0.25,  0.0f, 0.0f,
         0.25f,  0.75f,  0.25,  1.0f, 0.0f,

        -0.25f, -0.75f, -0.25,  0.0f, 1.0f,
         0.25f, -0.75f, -0.25,  1.0f, 1.0f,
         0.25f, -0.75f,  0.25,  1.0f, 0.0f,
         0.25f, -0.75f,  0.25,  1.0f, 0.0f,
        -0.25f, -0.75f,  0.25,  0.0f, 0.0f,
        -0.25f, -0.75f, -0.25,  0.0f, 1.0f,

        -0.25f,  0.75f, -0.25,  0.0f, 1.0f,
         0.25f,  0.75f, -0.25,  1.0f, 1.0f,
         0.25f,  0.75f,  0.25,  1.0f, 0.0f,
         0.25f,  0.75f,  0.25,  1.0f, 0.0f,
        -0.25f,  0.75f,  0.25,  0.0f, 0.0f,
        -0.25f,  0.75f, -0.25,  0.0f, 1.0f
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture 
    // -------------------------
    unsigned int texture1;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    //Load a texture for each frame
    int numTextures = 445;
    unsigned char* textureData[445]; 

    //Code for getting the correct file name
    for(int i =0; i<numTextures;i++){
        std::string sStr = "bu/bu-0000";
        if(i>=9){
            sStr = "bu/bu-000";
        }if(i>=99){
            sStr = "bu/bu-00";
        }
        std::string name =sStr+std::to_string(i+1)+".jpg"; 
        char const *pchar = name.c_str();
        textureData[i]= stbi_load(pchar, &width, &height,&nrChannels, 0);
    }

    //Load the first texture
    if (textureData[0])
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData[0]);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    


    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);


    int time = 0;
    float lastTime = 0;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!


        //Update the texture every .041 seconds to roughly sync up with the original speed
        float timeValue=glfwGetTime();
        if(timeValue-lastTime >= 0.041){
            //update to the next texture
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, textureData[time%numTextures]);
            time++;
            lastTime = timeValue;
        }

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        ourShader.use();

        glm::mat4 model         = glm::mat4(1.0f); 
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        ourShader.setMat4("projection", projection);

        // render box
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawArrays(GL_TRIANGLES, 0, 108);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}