#include <glad/glad.h>

#include "shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;

// Constructor & destructor
Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream fShaderFile;

  // Ensure ifstream objects can throw exceptions
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try
  {
    std::stringstream vShaderStream, fShaderStream;

    // Open files
    vShaderFile.open(vertexShaderPath);
    fShaderFile.open(fragmentShaderPath);

    // Read files buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    // Close file handlers
    vShaderFile.close();
    fShaderFile.close();

    // Convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch (std::ifstream::failure& e)
  {
    throw(string)"ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ";
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  unsigned int vertexShader, fragmentShader;

  // Vertex shader
  cout << "Creating vertex shader" << endl;
  // Create vertex shader object and compile
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vShaderCode, NULL);
  glCompileShader(vertexShader);
  checkCompilerErrors(vertexShader, "VERTEX");

  // Fragment shader
  cout << "Creating fragment shader" << endl;
  // Create fragment shader object and compile
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
  glCompileShader(fragmentShader);
  checkCompilerErrors(fragmentShader, "FRAGMENT");

  // Shader program
  cout << "Creating shader program" << endl;
  ID = glCreateProgram();
  // Attach shaders to program and link
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);
  checkCompilerErrors(ID, "PROGRAM");

  // Delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}
Shader::~Shader() {
  deactivate();
}

void Shader::activate()
{
  glUseProgram(ID);
}

void Shader::deactivate() {
  glDeleteProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::checkCompilerErrors(unsigned int uiHandle, std::string sType)
{
  int iSuccessCode;
  char cInfoLog[512];

  if (sType == "PROGRAM")
  {
    glGetProgramiv(uiHandle, GL_LINK_STATUS, &iSuccessCode);

    if (!iSuccessCode)
    {
      glGetProgramInfoLog(uiHandle, 512, NULL, cInfoLog);
      std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << sType << "\n"
        << cInfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
  else
  {
    glGetShaderiv(uiHandle, GL_COMPILE_STATUS, &iSuccessCode);

    if (!iSuccessCode)
    {
      glGetShaderInfoLog(uiHandle, 512, NULL, cInfoLog);
      std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << sType << "\n"
        << cInfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
}
