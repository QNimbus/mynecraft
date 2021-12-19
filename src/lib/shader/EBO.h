/* EBO.h */

#ifndef EBO_HEADER_H
#define EBO_HEADER_H

#include <glad/glad.h>

class EBO {
public:
  // ID reference of Elements Buffer Object
  GLuint ID;

  // Constructor & destructor
  EBO(GLuint* indices, GLsizeiptr size);

  void bind();
  void unbind();
  void remove();

};

#endif