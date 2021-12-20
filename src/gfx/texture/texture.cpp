#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

#include "texture.h"

Texture::Texture(const char* image, GLenum textureType, GLenum slot, GLenum format, GLenum pixelType) {
  // Assigns the type of texture to the texture object
  type = textureType;

  // Stores the width, height, and the number of color channels of the image
  int iImgWidth, iImgHeight, iNrColorChannels;

  // Flips the image so it appears right side up
  stbi_set_flip_vertically_on_load(true);

  // Reads the image from a file and stores it in bytes
  unsigned char* bytes = stbi_load(image, &iImgWidth, &iImgHeight, &iNrColorChannels, 0);

  // Generates an OpenGL texture object
  glGenTextures(1, &ID);

  // Assigns the texture to a Texture Unit
  glActiveTexture(slot);
  glBindTexture(textureType, ID);

  // Texture settings
  // Configures the type of algorithm that is used to make the image smaller or bigger
  glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Configures the way the texture repeats (if it does at all)
  glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  // In case of GL_CLAMP_TO_BORDER, define a color to fill the rest/untextured surface
  float flatColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glTexParameterfv(textureType, GL_TEXTURE_BORDER_COLOR, flatColor);

  // Assigns the image to the OpenGL Texture object
  glTexImage2D(textureType, 0, GL_RGBA, iImgWidth, iImgHeight, 0, format, pixelType, bytes);
  // Generates MipMaps
  glGenerateMipmap(textureType);

  // Deletes the image data as it is already in the OpenGL Texture object
  stbi_image_free(bytes);

  // Unbinds the OpenGL Texture object so that it can't accidentally be modified
  this->unbind();
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
  // Gets the location of the uniform
  GLuint texUniform = glGetUniformLocation(shader.ID, uniform);
  // Shader needs to be activated before changing the value of a uniform
  shader.activate();
  // Sets the value of the uniform
  glUniform1i(texUniform, unit);
}

void Texture::bind() {
  glBindTexture(type, ID);
}

void Texture::unbind() {
  glBindTexture(type, 0);
}

void Texture::remove() {
  glDeleteTextures(1, &ID);
}