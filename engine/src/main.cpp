#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include <memory>
#include <filesystem>


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

#ifdef ARCHRENDER_NATIVE_ADDON
extern "C" bool attachGLFWWindowToView(GLFWwindow* window, void* parentView);
extern "C" void resizeEmbeddedGLFWWindow(GLFWwindow* window, int x, int y, int width, int height);
#endif



const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
GLFWwindow* renderWindow = nullptr;
std::unique_ptr<Shader> renderShader;
std::unique_ptr<Model> renderModel;

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


extern "C" bool initializeRenderer(void* parentView, const char* projectRoot){
    if(!glfwInit()){
        std::cout<<"glfw is failed to initialised \n";
        return false ;
    } ;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , 4) ;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR , 1) ;
    glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE) ;

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
#ifdef ARCHRENDER_NATIVE_ADDON
    if(parentView){
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }
#endif

    // creating glfw window here
    renderWindow = glfwCreateWindow(SCR_WIDTH , SCR_HEIGHT , "ArchRenderer" , nullptr , nullptr) ;
    if(!renderWindow){
        std::cout<<"Window is failed to be created \n" ;
        glfwTerminate() ;
        return false ;
    }
    glfwMakeContextCurrent(renderWindow) ;

    // calling fucntions 
    glfwSetFramebufferSizeCallback(renderWindow, setWindowSize);
    glfwSetCursorPosCallback(renderWindow, mouse_callback);
    glfwSetScrollCallback(renderWindow, scroll_callback);
#ifdef ARCHRENDER_NATIVE_ADDON
    glfwSetInputMode(renderWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#else
    glfwSetInputMode(renderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

    // initialising glad here
    if(!gladLoadGL()){
        std::cout<<"Glad is failed to be initialised\n" ;
        glfwDestroyWindow(renderWindow);
        renderWindow = nullptr;
        glfwTerminate();
        return false ;
    } ;


    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    const std::filesystem::path root = projectRoot ? projectRoot : "";
    const std::string vertexShaderPath = projectRoot
        ? (root / "engine/shaders/vertex.glsl").string()
        : "engine/shaders/vertex.glsl";
    const std::string fragmentShaderPath = projectRoot
        ? (root / "engine/shaders/fragment.glsl").string()
        : "engine/shaders/fragment.glsl";
    const std::string modelPath = projectRoot
        ? (root / "models/backpack/backpack.obj").string()
        : "models/backpack/backpack.obj";
    renderShader = std::make_unique<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    // load models
    // -----------
    renderModel = std::make_unique<Model>(modelPath);

    //Model ourModel("models/houses/house 2.obj") ;

#ifdef ARCHRENDER_NATIVE_ADDON
    if(parentView){
        if(!attachGLFWWindowToView(renderWindow, parentView)){
            std::cerr<<"Failed to attach the OpenGL window to the Electron viewport.\n";
            renderModel.reset();
            renderShader.reset();
            glfwDestroyWindow(renderWindow);
            renderWindow = nullptr;
            glfwTerminate();
            return false;
        }
    }
#endif

    return true;
}

extern "C" void setRendererViewport(int x, int y, int width, int height){
    if(!renderWindow || width <= 0 || height <= 0){
        return;
    }

#ifdef ARCHRENDER_NATIVE_ADDON
    resizeEmbeddedGLFWWindow(renderWindow, x, y, width, height);
#endif
}

extern "C" bool rendererShouldClose(){
    return !renderWindow || glfwWindowShouldClose(renderWindow);
}

extern "C" void renderFrame(){
    if(!renderWindow || !renderShader || !renderModel){
        return;
    }

    glfwMakeContextCurrent(renderWindow);
    glfwPollEvents();
    if(glfwWindowShouldClose(renderWindow)){
        return;
    }

    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    getKey(renderWindow);

    int framebufferWidth;
    int framebufferHeight;
    glfwGetFramebufferSize(renderWindow, &framebufferWidth, &framebufferHeight);
    if(framebufferWidth <= 0 || framebufferHeight <= 0){
        return;
    }

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderShader->use();
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.Zoom),
        static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight),
        0.1f,
        100.0f
    );
    glm::mat4 view = camera.getView();
    renderShader->setMat4("projection", projection);
    renderShader->setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.06f));
    model = glm::translate(model, glm::vec3(-2.0171f, -13.215f, -0.7135f));
    renderShader->setMat4("model", model);
    renderModel->Draw(*renderShader);

    glfwSwapBuffers(renderWindow);
}

extern "C" void shutdownRenderer(){
    if(!renderWindow){
        return;
    }

    glfwMakeContextCurrent(renderWindow);
    renderModel.reset();
    renderShader.reset();
    glfwDestroyWindow(renderWindow);
    renderWindow = nullptr;
    glfwTerminate();
}

#ifndef ARCHRENDER_NATIVE_ADDON
int main(){
    if(!initializeRenderer(nullptr, nullptr)){
        return 1;
    }

    while(!rendererShouldClose()){
        renderFrame();
    }

    shutdownRenderer();
    return 0;
}
#endif