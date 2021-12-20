/* VBO.h */

#ifndef VBO_HEADER_H
#define VBO_HEADER_H

#include <glad/glad.h>

class VBO {
public:
  GLuint ID;

  // Constructor & destructor
  VBO(GLfloat* vertices, GLsizeiptr size);

  void bind();
  void unbind();
  void remove();

};

#endif