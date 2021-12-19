/* VAO.h */

#ifndef VAO_HEADER_H
#define VAO_HEADER_H

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
  GLuint ID;

  // Constructor & destructor
  VAO();

  void linkVBO(VBO VBO, GLuint layout);
  void bind();
  void unbind();
  void remove();

};

#endif