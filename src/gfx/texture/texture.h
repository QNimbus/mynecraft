/* texture.h */

#ifndef TEXTURE_HEADER_H
#define TEXTURE_HEADER_H

#include <glad/glad.h>

#include "../shader/shader.h"

class Texture {
public:
  // ID reference to the OpenGL texture that was generated
  GLuint ID;
  GLenum type;

  // Constructor & destructor
  Texture(const char* image, GLenum textureType, GLenum slot, GLenum format, GLenum pixelType);

  // Assigns a texture unit to a texture
  void texUnit(Shader& shader, const char* uniform, GLuint unit);
  void bind();
  void unbind();
  void remove();
};

#endif