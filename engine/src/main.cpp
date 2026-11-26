#include<iostream>
#include<string>


#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// include made by myself present in the includes 
#include <camera.hpp>
#include <shaders.hpp>
#include<model.hpp>

//image tool 
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera Object
Camera camera(glm::vec3(0.0f , 3.0f , 15.0f)) ;
float lastX = SCR_WIDTH / 2.0f ;
float lastY = SCR_HEIGHT / 2.0f ;
bool firstMouse = true ;

//timings 
static float deltaTime = 0.0f ;
float lastFrame = 0.0f ;





// set window size 
void setWindowSize(GLFWwindow* window , int width , int height){
    glViewport(0 , 0 , width , height) ;
}


// model matrix 
const glm::mat4 vampireModelTransform =
    glm::translate(glm::mat4(1.0f), glm::vec3(0.00003f, -1.04077f, -0.02645f)) *
    glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
glm::mat4 model = vampireModelTransform;


static bool shouldRotate = false;
static bool rotateKeyWasPressed = false;

// rotate on the y axis
void rotating(){
    if(shouldRotate){
        model = glm::rotate(model, glm::radians(45.0f * deltaTime), glm::vec3(0.0f, 1.0f, 0.0f));
    }
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
    if(glfwGetKey(window , GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        camera.ProcessKeyboard(FASTFORWARD , deltaTime) ;
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
    if(glfwGetKey(window , GLFW_KEY_I) == GLFW_PRESS){
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -deltaTime * 3.0f)) * model;
    }
    if(glfwGetKey(window , GLFW_KEY_K) == GLFW_PRESS){
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, deltaTime * 3.0f)) * model;
    }
    if(glfwGetKey(window , GLFW_KEY_L) == GLFW_PRESS){
        model = glm::translate(glm::mat4(1.0f), glm::vec3(deltaTime * 3.0f, 0.0f, 0.0f)) * model;
    }
    if(glfwGetKey(window , GLFW_KEY_J) == GLFW_PRESS){
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-deltaTime * 3.0f, 0.0f, 0.0f)) * model;
    }
    if(glfwGetKey(window , GLFW_KEY_SPACE) == GLFW_PRESS){
        model = vampireModelTransform;
    }

    const bool rotateKeyIsPressed = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
    if(rotateKeyIsPressed && !rotateKeyWasPressed){
        shouldRotate = !shouldRotate;
    }
    rotateKeyWasPressed = rotateKeyIsPressed;
    rotating();

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



// load skybox texture

unsigned int loadCubemap(std::vector<std::string> &faces){
    unsigned int textureId ;
    glGenTextures(1 , &textureId) ;
    glBindTexture(GL_TEXTURE_CUBE_MAP , textureId) ;
    int width , height , nrChannels ;
    for(int i = 0 ; i < faces.size() ; i++){
        unsigned char *data = stbi_load(faces[i].c_str() , &width , &height , &nrChannels , 0) ;
        if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureId ;
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


    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST) ;


    // loading shder files
    const std::string vertexPath = "engine/shaders/vertex.glsl";
    const std::string fragmentPath = "engine/shaders/fragment.glsl";

    
    Model *ourmodel = new Model("models/vampire/dancing_vampire.dae") ;

    Model *sunmodel = new Model("models/sun/model.glb") ;

    if(ourmodel->isEmpty()){
        std::cerr << "Failed to load model ourmodel\n";
        delete ourmodel;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    if(sunmodel->isEmpty()){
        std::cerr<<"Failed to load model : sun model\n" ;
        delete sunmodel ;
        glfwDestroyWindow(window) ;
        glfwTerminate() ;
        return -1 ;
    }


    // class shder class 
    Shader *shaderProgram  ;
    
    try {
        shaderProgram = new Shader(vertexPath , fragmentPath) ;
    } catch (const std::exception& error) {
        std::cerr << "Failed to create shader program: " << error.what() << '\n';
        glfwDestroyWindow(window) ;
        glfwTerminate() ;
        return -1 ;
    }

    Shader *sunshader = new Shader(
        "engine/shaders/sunvertex.glsl",
        "engine/shaders/sunfragment.glsl"
    );


    // skybox shader 
    Shader *skyshader = new Shader (
        "engine/shaders/cubemapvert.glsl" ,
        "engine/shaders/cubemapfrag.glsl"
    );

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);


    projection = glm::perspective(glm::radians(camera.Zoom),(float) SCR_WIDTH / (float) SCR_HEIGHT,0.1f,400.0f);


    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);



    // get loaction for the model 
    const GLint modelLoc = glGetUniformLocation(shaderProgram->ID, "model");
    const glm::vec3 sunPosition = glm::vec3(0.0f , 30.0f , -320.0f) ;
    glm::mat4 sunPos = glm::translate(glm::mat4(1.0f), sunPosition);
    sunPos = glm::scale(sunPos, glm::vec3(0.015f));


    //load texture images path 
    // std::vector<std::string> path = {
    //     "textures/right.jpg",
    //     "textures/left.jpg",
    //     "textures/top.jpg",
    //     "textures/bottom.jpg",
    //     "textures/front.jpg",
    //     "textures/back.jpg"
    // };

        std::vector<std::string> path = {
        "textures/black_space/top.jpg",
        "textures/black_space/top.jpg",
        "textures/black_space/top.jpg",
        "textures/black_space/top.jpg",
        "textures/black_space/top.jpg",
        "textures/black_space/top.jpg"
    };

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    unsigned int skyVAO , skyVBO ;

    glGenVertexArrays(1 , &skyVAO) ;
    glGenBuffers(1 , &skyVBO) ;
    glBindVertexArray(skyVAO) ;
    glBindBuffer(GL_ARRAY_BUFFER , skyVBO) ;
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    // fix te inverted image error fir turn off the invertion 
    stbi_set_flip_vertically_on_load(0) ;
    unsigned int cubeMap = loadCubemap(path) ;
    // again turn on the invertion 
    stbi_set_flip_vertically_on_load(1) ;




    while(!glfwWindowShouldClose(window)){
        float currentFrame = static_cast<float>(glfwGetTime()) ;
        deltaTime = currentFrame - lastFrame ;
        lastFrame = currentFrame ;
        
        getKey(window) ;
        glClearColor(0.24f, 0.27f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;

        shaderProgram->use() ;
        
        const glm::vec3 lightDirection = glm::normalize(sunPosition);
        glUniform3f(
            glGetUniformLocation(shaderProgram->ID, "lightDirection"),
            lightDirection.x, lightDirection.y, lightDirection.z
        );
        glUniform3f(
            glGetUniformLocation(shaderProgram->ID, "lightColor"),
            1.8f, 1.65f, 1.4f
        );
        glUniform3f(
            glGetUniformLocation(shaderProgram->ID, "viewPos"),
            camera.Position.x, camera.Position.y, camera.Position.z
        );
        
        view = camera.getView();
        glUniformMatrix4fv(
            glGetUniformLocation(shaderProgram->ID, "view"),
            1, GL_FALSE, glm::value_ptr(view)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(shaderProgram->ID, "projection"),
            1, GL_FALSE, glm::value_ptr(projection)
        );

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        ourmodel->Draw(*shaderProgram);


        sunshader->use();
        glUniformMatrix4fv(
            glGetUniformLocation(sunshader->ID, "model"),
            1, GL_FALSE, glm::value_ptr(sunPos)
        );
        glUniformMatrix4fv(
            glGetUniformLocation(sunshader->ID, "view"),
            1, GL_FALSE, glm::value_ptr(view)
        );
        glUniformMatrix4fv(
            glGetUniformLocation(sunshader->ID, "projection"),
            1, GL_FALSE, glm::value_ptr(projection)
        );
        sunmodel->Draw(*sunshader);

        skyshader->use();
        const glm::mat4 skyView = glm::mat4(glm::mat3(view));
        glUniformMatrix4fv(
            glGetUniformLocation(skyshader->ID, "view"),
            1, GL_FALSE, glm::value_ptr(skyView)
        );
        glUniformMatrix4fv(
            glGetUniformLocation(skyshader->ID, "projection"),
            1, GL_FALSE, glm::value_ptr(projection)
        );
        glUniform1i(glGetUniformLocation(skyshader->ID, "skybox"), 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        glBindVertexArray(skyVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window) ;
        glfwPollEvents() ;
    }

    glDeleteBuffers(1 , &skyVBO) ;
    glDeleteVertexArrays(1 , &skyVAO) ;
    delete skyshader ;
    delete shaderProgram ;
    delete sunshader;
    delete ourmodel;
    delete sunmodel;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}