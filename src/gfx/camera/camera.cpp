#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position) {
  this->width = width;
  this->height = height;

  this->position = position;
}

void Camera::Matrix(float fFOVdeg, float fNearPlane, float fFarPlane, Shader& shader, const char* uniform) {
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  view = glm::lookAt(this->position, this->position * this->orientation, this->up);
  projection = glm::perspective(glm::radians(fFOVdeg), (float)(this->width / this->height), fNearPlane, fFarPlane);

  glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    this->position += this->speed * this->orientation;
  };
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    this->position += this->speed * -glm::normalize(glm::cross(this->orientation, this->up));
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    this->position += this->speed * -this->orientation;
  };
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    this->position += this->speed * glm::normalize(glm::cross(this->orientation, this->up));
  }
}