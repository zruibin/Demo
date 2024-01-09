#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    unsigned int ID;
    
    virtual ~Shader() 
    {
        // std::cout<<"Shader Destructor"<<std::endl;
    }

    Shader() = default;
    
    Shader(const char* vertexPath,
           const char* fragmentPath,
           const char* geometryPath = nullptr) 
    {

        if (vertexPath != nullptr) {
            this->vertexCode = this->readShaderFromFile(vertexPath);
        }
        if (fragmentPath != nullptr) {
            this->fragmentCode = this->readShaderFromFile(fragmentPath);
        }
        if (geometryPath != nullptr) {
            this->geometryCode = this->readShaderFromFile(geometryPath);
        }
//        std::cout<< this->vertexCode << std::endl;
//        std::cout<< this->fragmentCode << std::endl;
        this->linkProgram();
    }

    Shader(std::string vCode,
           std::string fCode,
           std::string gCode = ""):vertexCode(vCode),
        fragmentCode(fCode), geometryCode(gCode)
    {
        this->linkProgram();
    }

    // activate the shader
    void use() 
    { 
        glUseProgram(ID);
    }

    // utility uniform functions
    void setBool(const std::string &name, bool value) const 
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }

    void setInt(const std::string &name, int value) const 
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void setFloat(const std::string &name, float value) const 
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }

    void setVec2(const std::string &name, const glm::vec2 &value) const 
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const 
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    
    void setVec3(const std::string &name, const glm::vec3 &value) const 
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    
    void setVec4(const std::string &name, const glm::vec4 &value) const 
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) 
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    
    void setMat2(const std::string &name, const glm::mat2 &mat) const 
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
    void setMat3(const std::string &name, const glm::mat3 &mat) const 
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
    void setMat4(const std::string &name, const glm::mat4 &mat) const 
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:

    void linkProgram()
    {
        const char* vShaderCode = this->vertexCode.c_str();
        const char * fShaderCode = this->fragmentCode.c_str();
        
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if (geometryCode != "") {
            const char * gShaderCode = this->geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }

        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if(geometryCode != "")
            glAttachShader(ID, geometry);
        
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(geometryCode != "")
            glDeleteShader(geometry);
    }

    void checkCompileErrors(GLuint shader, std::string type) 
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
    
    std::string readShaderFromFile(const char* path)
    {
        if (path == nullptr) {
            return "";
        }
        std::string shaderCode;
        std::ifstream vShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vShaderFile.open(path);
            std::stringstream vShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            vShaderFile.close();
            shaderCode = vShaderStream.str();
//            std::cout<<shaderCode<<std::endl;
        } catch (std::ios_base::failure& e) {
            std::cout << e.what() << std::endl;
            if (errno) {
                std::cout << strerror(errno) << std::endl;
            }
            std::cout << "Caught an ios_base::failure.\n"
                << "Error code: " << e.code().value()
                << " (" << e.code().message() << ")\n"
                << "Error category: " << e.code().category().name() << std::endl;
        }
        return shaderCode;
    }

private:
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
};
#endif
