/* shader.h */

#ifndef SHADER_HEADER_H
#define SHADER_HEADER_H

#include <glad/glad.h>
#include <string>

class Shader
{
public:
  GLuint ID;

  // Constructor & destructor
  Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
  ~Shader();

  // Activate & Deactivate shader
  void activate();
  void deactivate();

  // Utility uniform methods
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;

private:
  void checkCompilerErrors(unsigned int shader, std::string type);
};

#endif