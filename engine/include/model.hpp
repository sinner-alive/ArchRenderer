#pragma once
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.hpp>
#include <shaders.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <filesystem>
#include <limits>

inline unsigned int TempTextureFromFile(const char *path, const std::string &directory)
{
    std::filesystem::path filename(path);
    if(filename.is_relative())
        filename = std::filesystem::path(directory) / filename;

    unsigned int textureID = 0;
    glGenTextures(1, &textureID);

    int width = 0;
    int height = 0;
    int components = 0;
    unsigned char *data = stbi_load(filename.string().c_str(), &width, &height, &components, 0);
    if(!data)
    {
        std::cerr << "Texture failed to load at path: " << filename
                  << " (" << stbi_failure_reason() << ')' << std::endl;
        glDeleteTextures(1, &textureID);
        return 0;
    }

    GLenum format = 0;
    if(components == 1)
        format = GL_RED;
    else if(components == 3)
        format = GL_RGB;
    else if(components == 4)
        format = GL_RGBA;

    if(format == 0)
    {
        std::cerr << "Unsupported texture format at path: " << filename << std::endl;
        stbi_image_free(data);
        glDeleteTextures(1, &textureID);
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);
    return textureID;
}

inline unsigned int TempTextureFromRGBA(const unsigned char *data, int width, int height)
{
    unsigned int textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return textureID;
}

class Model {
    public:
    Model(const std::string &path){
        loadModel(path) ;
    }
    void Draw(Shader &shader){
        for(Mesh &mesh : meshes)
            mesh.Draw(shader);
    }
    bool isEmpty(){
        if(meshes.empty()){
            return true;
        }
        return false;
    }

    private :
    std::vector<Mesh> meshes ;
    std::vector<Texture> textures_loaded ;
    std::string directory ;
    void loadModel(const std::string &path){
        Assimp::Importer import ;
        const std::string extension = std::filesystem::path(path).extension().string();
        unsigned int importFlags = aiProcess_Triangulate;
        if(extension != ".glb" && extension != ".gltf")
            importFlags |= aiProcess_FlipUVs;
        const aiScene *scene = import.ReadFile(path, importFlags) ;
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            std::cerr << "ERROR::ASSIMP:: " << import.GetErrorString() << std::endl;
            return;
        }
        directory = std::filesystem::path(path).parent_path().string();

        processNode(scene->mRootNode, scene);
    }
    void processNode(aiNode *node , const aiScene *scene){
        // process all the node's meshes (if any)
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
            meshes.push_back(processMesh(mesh, scene));			
        }
        // then do the same for each of its children
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }
    Mesh processMesh(aiMesh *mesh , const aiScene *scene){
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for(unsigned int i = 0; i < mesh->mNumVertices; i++){
            Vertex vertex{};
            vertex.Position = glm::vec3(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            );

            if(mesh->HasNormals()){
                vertex.Normal = glm::vec3(
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
                );
            }

            if(mesh->mTextureCoords[0]){
                vertex.texture = glm::vec2(
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                );
            } else {
                vertex.texture = glm::vec2(0.0f);
            }

            vertices.push_back(vertex);
        }

        for(unsigned int i = 0; i < mesh->mNumFaces; i++){
            const aiFace &face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        if(mesh->mMaterialIndex < scene->mNumMaterials){
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            const aiTextureType diffuseType =
                material->GetTextureCount(aiTextureType_BASE_COLOR) > 0
                    ? aiTextureType_BASE_COLOR
                    : aiTextureType_DIFFUSE;
            std::vector<Texture> diffuseMaps =
                loadMaterialTextures(material, scene, diffuseType, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<Texture> specularMaps =
                loadMaterialTextures(material, scene, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> loadMaterialTextures(
        aiMaterial *mat,
        const aiScene *scene,
        aiTextureType type,
        const std::string &typeName
    ){
        std::vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
            aiString texturePath;
            if(mat->GetTexture(type, i, &texturePath) != AI_SUCCESS)
                continue;

            bool alreadyLoaded = false;
            for(const Texture &loadedTexture : textures_loaded){
                if(loadedTexture.path == texturePath.C_Str()){
                    textures.push_back(loadedTexture);
                    alreadyLoaded = true;
                    break;
                }
            }

            if(!alreadyLoaded){
                Texture texture{};
                const aiTexture *embeddedTexture = scene->GetEmbeddedTexture(texturePath.C_Str());
                if(embeddedTexture && embeddedTexture->mHeight == 0){
                    if(embeddedTexture->mWidth <= static_cast<unsigned int>(std::numeric_limits<int>::max())){
                        int width = 0;
                        int height = 0;
                        int components = 0;
                        unsigned char *decoded = stbi_load_from_memory(
                            reinterpret_cast<const unsigned char *>(embeddedTexture->pcData),
                            static_cast<int>(embeddedTexture->mWidth),
                            &width,
                            &height,
                            &components,
                            STBI_rgb_alpha
                        );
                        if(decoded){
                            texture.id = TempTextureFromRGBA(decoded, width, height);
                            stbi_image_free(decoded);
                        } else {
                            std::cerr << "Embedded texture failed to decode: "
                                      << texturePath.C_Str() << " ("
                                      << stbi_failure_reason() << ')' << std::endl;
                        }
                    } else {
                        std::cerr << "Embedded texture is too large to decode: "
                                  << texturePath.C_Str() << std::endl;
                    }
                } else if(embeddedTexture){
                    const std::size_t texelCount =
                        static_cast<std::size_t>(embeddedTexture->mWidth) * embeddedTexture->mHeight;
                    if(texelCount <= std::numeric_limits<std::size_t>::max() / 4){
                        std::vector<unsigned char> rgba(texelCount * 4);
                        for(std::size_t pixel = 0; pixel < texelCount; ++pixel){
                            rgba[pixel * 4] = embeddedTexture->pcData[pixel].r;
                            rgba[pixel * 4 + 1] = embeddedTexture->pcData[pixel].g;
                            rgba[pixel * 4 + 2] = embeddedTexture->pcData[pixel].b;
                            rgba[pixel * 4 + 3] = embeddedTexture->pcData[pixel].a;
                        }
                        texture.id = TempTextureFromRGBA(
                            rgba.data(),
                            static_cast<int>(embeddedTexture->mWidth),
                            static_cast<int>(embeddedTexture->mHeight)
                        );
                    }
                } else {
                    texture.id = TempTextureFromFile(texturePath.C_Str(), directory);
                }
                texture.type = typeName;
                texture.path = texturePath.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
        return textures;
    }

} ;