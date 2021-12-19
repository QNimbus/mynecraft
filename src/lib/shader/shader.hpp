/* shader.hpp */

#ifndef SHADER_HEADER_H
#define SHADER_HEADER_H

#include <string>

class Shader
{
public:
  unsigned int ID;

  // Constructor & destructor
  Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
  ~Shader();

  // Activate shader
  void use();
  // Utility uniform methods
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;

private:
  void checkCompilerErrors(unsigned int shader, std::string type);
};

#endif