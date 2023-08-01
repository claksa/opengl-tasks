#ifndef GRAPHICS_SHADER_HPP
#define GRAPHICS_SHADER_HPP

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <filesystem>
#include <iterator>

#include <sstream>
#include <iostream>

class Shader {
private:
    // utility function for checking shader compilation/linking errors
    static void checkCompileErrors(unsigned int shader, const std::string& type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" <<
                infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" <<
                infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    static std::string readShaderFile(const char* shaderFile) {
        std::ifstream fs;
        auto ex = fs.exceptions();
        ex |= std::ios_base::badbit | std::ios_base::failbit;
        fs.exceptions(ex);

        fs.open(shaderFile);
        fs.ignore(std::numeric_limits<std::streamsize>::max());
        auto size = fs.gcount();

        if (size > 0x10000) // 64KiB sanity check for shaders: ??
            return {};

        fs.clear();
        fs.seekg(0, std::ios_base::beg);

        return std::string {std::istreambuf_iterator<char> {fs}, {}};
    }


public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
     Shader(const char *vertexPath,
            const char *fragmentPath) {
        std::string vertexCode;
        std::string fragmentCode;
        // 1. retrieve the vertex/fragment source code from filePath
        try {

            vertexCode = readShaderFile(vertexPath);
            fragmentCode = readShaderFile(fragmentPath);

        } catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();

        // 2. compile shaders

        // vertex shader
        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);

        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // use/activate the shader
    void use() {
         glUseProgram(ID);
     }

    // utility uniform functions
    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

};

#endif //GRAPHICS_SHADER_HPP
