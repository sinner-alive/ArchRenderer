#pragma once 

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shaders.hpp>

#include <string>
#include <vector>
using namespace std;


struct Vertex {
    glm::vec3 Position ;
    glm::vec3 Normal ;
    glm::vec2 texture ;
};

struct Texture {
    unsigned int id ;
    string type ; // diffuse or specular texture 
    string path ;
};

class Mesh {
    public :
    vector<Vertex> vertices ;
    vector<unsigned int > indices ;
    vector<Texture> textures ;

    Mesh(vector<Vertex>vertices , vector<unsigned int> indices , vector<Texture> textures){
        this->vertices = vertices ;
        this->indices =indices ;
        this->textures = textures ;

        setUpMesh() ;
    }
    
    void Draw(Shader &shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        bool hasDiffuseTexture = false;
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = textures[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);

            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
            if(name == "texture_diffuse" && textures[i].id != 0)
                hasDiffuseTexture = true;
        }
        glUniform1i(glGetUniformLocation(shader.ID, "hasDiffuseTexture"), hasDiffuseTexture);
        glUniform3f(glGetUniformLocation(shader.ID, "materialDiffuse"), 1.0f, 1.0f, 1.0f);
        glActiveTexture(GL_TEXTURE0);

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }  
    private :
    unsigned int VAO , VBO , EBO ;
    void setUpMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                    &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

        glBindVertexArray(0);
    }  
} ;
