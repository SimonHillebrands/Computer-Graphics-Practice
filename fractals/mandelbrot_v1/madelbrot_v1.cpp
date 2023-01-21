#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_s.h>

#include <math.h>
#include <iostream>

#include <vector> 
using namespace std;

//prototype for callback function for when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int mandelbrot(int xPos, int yPos);
float scale(int m, float rmin, float rmax, float tmin, float tmax);
// settings
const unsigned int SCR_WIDTH = 400;
const unsigned int SCR_HEIGHT = 360;
const int ITERATIONS = 1000;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Mandelbrot", NULL, NULL);
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

    Shader ourShader("shader.vs", "shader.fs"); 

    int totalPoints = SCR_HEIGHT*SCR_WIDTH;

    //2d vector, each row contains a vector of length 6, first three points are pos, next three are color
    vector<vector<float>> pointsV( totalPoints , vector<float> (6, 0.0f)); 
    int counter = 0;

    for(int i = 0; i < SCR_HEIGHT; i++){
        for(int j = 0; j < SCR_WIDTH; j++){
           pointsV[counter][0] = scale(j, 0, SCR_WIDTH, -1, 1);  
           pointsV[counter][1] = scale(i, 0, SCR_HEIGHT, -1, 1);
           pointsV[counter][2] = 0.0f;

            int brot = mandelbrot(j,i);
            
            //Color linearly based on number of iterations
            pointsV[counter][3] = scale(brot,0,ITERATIONS,1.0,0);  
            pointsV[counter][4] = scale(brot,0,ITERATIONS,1.0,0); 
            pointsV[counter][5] = scale(brot,0,ITERATIONS,1.0,0);


            //Change the color regularly based on the iteration to create a colored pattern
            // if(brot%2 == 0){
            //     pointsV[counter][3] = 0.2f;
            // }
            // if(brot%3 == 0){
            //     pointsV[counter][3] = 0.5f;
            // }


            // if(brot%7 == 0){
            //     pointsV[counter][4] = 0.2f;
            // }
            // if(brot%9 == 0){
            //     pointsV[counter][4] = 0.5f;
            // }
            // if(brot%11 == 0){
            //     pointsV[counter][4] = 1.0f;
            // }
            // if(brot%4 == 0){
            //     pointsV[counter][5] = 0.2f;
            // }
            // if(brot%6 == 0){
            //     pointsV[counter][5] = 0.5f;
            // }

            counter++;

        }
    }

   unsigned int VBO[SCR_HEIGHT*SCR_WIDTH], VAO[SCR_HEIGHT*SCR_WIDTH];
   glGenVertexArrays(totalPoints, VAO);

       glGenBuffers(totalPoints, VBO);

    for(int i = 0; i < totalPoints; i++){

        //This feels pretty bad and stupid but it was easier to work with super large vectors over arrays
        //im sure this was all done in a highly suboptimal way so here we are
        float vertices[6];
        for(int k = 0; k<6; k++){
            vertices[k] = pointsV[i][k];

        }

        glBindVertexArray(VAO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
        (void*)0);
        glEnableVertexAttribArray(0);

        //color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
        (void*)(3* sizeof(float)));
        glEnableVertexAttribArray(1);
        
    }
  

glBindBuffer(GL_ARRAY_BUFFER, 0); 
    //Render loop
    cout <<"uwu2"<<endl;
    while(!glfwWindowShouldClose(window))
    {

        processInput(window);


        //clear the buffer with this color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        for(int i = 0; i < totalPoints; i++){
                glBindVertexArray(VAO[i]); 
                glDrawArrays(GL_POINTS, 0, 1);     
            
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

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
float scale(int m, float rmin, float rmax, float tmin, float tmax){
    return( ((m - rmin)/(rmax - rmin)) * (tmax-tmin) + tmin);
}
int mandelbrot(int xPos, int yPos){
    float x0 = scale(xPos,0, SCR_WIDTH, -2.0, 0.47);
    float y0 = scale(yPos, 0, SCR_HEIGHT, -1.12, 1.12);

    float x = 0.0;
    float y = 0.0;

    int iteration = 0;
    int max_iteration = ITERATIONS;

    while( (x*x) + (y*y) <= 4 && iteration < max_iteration){
        float xtemp = ((x*x) - (y*y)) + x0;
        y = (2*x*y) +y0;
        x = xtemp;
        iteration++;
    }
    return iteration;
}