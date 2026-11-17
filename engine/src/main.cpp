#include<iostream>
#include<fstream>
#include<sstream>
#include<string>


#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// include made by myself present in the includes 
#include <camera.hpp>
#include <shaders.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include<model.hpp>




const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera Object
Camera camera(glm::vec3(0.0f , 0.0f , 3.0f)) ;
float lastX = SCR_WIDTH / 2.0f ;
float lastY = SCR_HEIGHT / 2.0f ;
bool firstMouse = true ;


//timings 
float deltaTime = 0.0f ;
float lastFrame = 0.0f ;



// set window size 
void setWindowSize(GLFWwindow* window , int width , int height){
    glViewport(0 , 0 , width , height) ;
}

void rotate(float deltaTime){

}

// get window close input
void getKey(GLFWwindow *window){
    if(glfwGetKey(window , GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout<<"ESCAPE KEY IS PRESSED : WINDOW CLOSE \n" ;
        glfwSetWindowShouldClose(window , true) ;
    }
    if(glfwGetKey(window , GLFW_KEY_W) == GLFW_PRESS){
        camera.ProcessKeyboard(FORWARD , deltaTime) ;
    }
    if(glfwGetKey(window , GLFW_KEY_S) == GLFW_PRESS){
        camera.ProcessKeyboard(BACKWARD , deltaTime) ;
    }
    if(glfwGetKey(window , GLFW_KEY_A) == GLFW_PRESS){
        camera.ProcessKeyboard(LEFT , deltaTime) ;
    }
    if(glfwGetKey(window , GLFW_KEY_D) == GLFW_PRESS){
        camera.ProcessKeyboard(RIGHT , deltaTime) ;
    }
    if(glfwGetKey(window , GLFW_KEY_Q) == GLFW_PRESS){
        camera.ProcessKeyboard(UP , deltaTime) ;
    }
    if(glfwGetKey(window , GLFW_KEY_E) == GLFW_PRESS){
        camera.ProcessKeyboard(DOWN , deltaTime) ;
    }
}

// for mouse 
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{   
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


int main(){
    if(!glfwInit()){
        std::cout<<"glfw is failed to initialised \n";
        return 0 ;
    } ;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , 4) ;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR , 1) ;
    glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE) ;

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // creating glfw window here
    GLFWwindow *window = glfwCreateWindow(1920 , 1080 , "ArchRenderer" , nullptr , nullptr) ;
    if(!window){
        std::cout<<"Window is failed to be created \n" ;
        glfwTerminate() ;
        return 0 ;
    }
    glfwMakeContextCurrent(window) ;

    // calling fucntions 
    glfwSetFramebufferSizeCallback(window, setWindowSize);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // initialising glad here
    if(!gladLoadGL()){
        std::cout<<"Glad is failed to be initialised\n" ;
        glfwTerminate() ;
        return 0 ;
    } ;


    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("engine/shaders/vertex.glsl", "engine/shaders/fragment.glsl");

    // load models
    // -----------
    Model ourModel("models/backpack/backpack.obj");

    //Model ourModel("models/houses/house 2.obj") ;

    
    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        getKey(window) ;

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getView();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.06f));
        model = glm::translate(model, glm::vec3(-2.0171f, -13.215f, -0.7135f));
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}