#pragma once 

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<camera.hpp>
#include<GLFW/glfw3.h>

class UIMANAGER {
    public :
    void uiInit(GLFWwindow* window) ;
    void render(Camera &camera , glm::mat4 &model , bool &shouldRotate , glm::vec3& sunPos , glm::vec3& lightColor);

    void shutdown() ;
};

