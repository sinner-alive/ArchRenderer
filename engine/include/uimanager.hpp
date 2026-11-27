#pragma once 

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<camera.hpp>
#include<GLFW/glfw3.h>

#include<string>

class Model ;

class UIMANAGER {
    public :
    void uiInit(GLFWwindow* window) ;
    void render(Camera &camera , glm::mat4 &model , bool &shouldRotate , glm::vec3& sunPos , glm::vec3& lightColor , Model*& currentModel) ;

    void shutdown() ;

    private :
    void applyStyle() ;
    std::string browseForModel() ;
    void loadModelFromPath(const std::string& path , Model*& currentModel , glm::mat4 &model) ;

    std::string currentModelName = "Startup model" ;
    std::string currentModelPath ;
    std::string statusMessage = "Browse for a model to get started." ;
    bool statusIsError = false ;
    bool showInspector = true ;
    glm::mat4 initialModel = glm::mat4(1.0f) ;
    bool hasInitialModel = false ;
};
