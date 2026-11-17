#pragma once 

#include<iostream>
#include<glad/glad.h>
#include<fstream>
#include<sstream>
#include<string>
#include<stdexcept>


class Shader {
    public :

    unsigned int ID ;

    Shader(const std::string &vertexPath , const std::string &fragmentPath){
        const std::string vertexSource = loadShaderFile(vertexPath) ;
        const std::string fragmentSource = loadShaderFile(fragmentPath) ;
        unsigned int vertexShader = compileShader(GL_VERTEX_SHADER , vertexSource) ;
        unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER , fragmentSource) ;

        ID = glCreateProgram() ;
        glAttachShader(ID , vertexShader) ;
        glAttachShader(ID , fragmentShader) ;

        glLinkProgram(ID) ;
        // check program link status 
        int success ;
        char loginfo[512] ;
        glGetProgramiv(ID , GL_LINK_STATUS , &success) ;
        if(!success){
            glGetProgramInfoLog(ID , 512 , nullptr , loginfo) ;
            std::cerr<<"Failed to Link program : "<<loginfo<<"\n" ;
            glDeleteShader(vertexShader) ;
            glDeleteShader(fragmentShader) ;
            throw std::runtime_error("failed to link program") ;

        }
        glDeleteShader(vertexShader) ;
        glDeleteShader(fragmentShader) ;
    }

    // load shader source code 
    std::string loadShaderFile(const std::string &path){
        std::ifstream file(path) ;
        if(!file.is_open()){
            throw std::runtime_error("Failed to open shader file: " + path);
        }
        std::stringstream buffer ;
        buffer << file.rdbuf() ;
        return buffer.str() ;
    }

    unsigned int compileShader(unsigned int shaderType , const std::string &codeString){
        unsigned int currentShader = glCreateShader(shaderType) ;
        const char *shaderSource = codeString.c_str() ;
        glShaderSource(currentShader , 1 , &shaderSource , nullptr) ;
        glCompileShader(currentShader) ;
        int success ;
        char loginfo[512] ;
        glGetShaderiv(currentShader , GL_COMPILE_STATUS , &success) ;
        if(!success){
            glGetShaderInfoLog(currentShader , 512 , nullptr , loginfo) ;
            std::cerr<<"Shader Compilation Failed : "<<loginfo<<"\n" ;
            glDeleteShader(currentShader) ;
            throw std::runtime_error("Shader compilation failed") ;
        }
        return currentShader ;
    }
    void use(){
        glUseProgram(ID) ;
    }
    ~Shader (){
        if(ID != 0) 
        glDeleteProgram(ID) ;
    }
} ;
