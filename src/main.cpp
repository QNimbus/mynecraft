#include <math.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "lib/shader/shader.hpp"

using namespace std;

#define APP_VERSION "0.1.0"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

int main(int argc, char* argv[])
{
  // Variables used in this scope
  GLFWwindow* window;
  Shader* shader;

  try
  {

    /**
   * Before most GLFW functions can be used, GLFW must be initialized, and before an application terminates GLFW should
   * be terminated in order to free any resources allocated during or after initialization.
   *
   * see: https://www.glfw.org/docs/3.3/group__init.html#ga317aac130a235ab08c6db0834907d85e
  **/
    glfwInit();

    // Tell GLFW the version of OpenGL we are using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we will be using the CORE profile (since OpenGL 3.2, seeL https://en.wikipedia.org/wiki/OpenGL#OpenGL_3.2)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLfloat vertices[] = {
      -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
       0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
       0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
       -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
       0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
       0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Inner bottom
    };

    GLuint indices[] = {
      0, 3, 5, // Lower left triangle
      2, 3, 4, // Lower right triangle
      5, 4, 1, // Upper triangle
    };

    // Initialize window and check for success
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Mynecraft", NULL, NULL);
    if (!window)
    {
      cout << "Failed to create GLFW window" << endl;
      glfwTerminate();
      return EXIT_FAILURE;
    }

    // Add newly created window to current context
    glfwMakeContextCurrent(window);

    // GLAD is an OpenGL Loading Library is a library that loads pointers to OpenGL functions at runtime, core as well as extensions.
    // Load GLAD so it configures OpenGL
    gladLoadGL();

    // Output basic engine info
    const GLubyte* renderer = glGetString(GL_RENDERER); // Get renderer string
    const GLubyte* version = glGetString(GL_VERSION);   // Get OpenGL version as a string
    cout << "Mynecraft version: " << APP_VERSION << endl;
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported: " << version << endl;

    // Configure the viewport used by OpenGL in the window
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Specify clear values for the color buffers
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Actually perform clearing of the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);

    // Construct shader object
    shader = new Shader("./src/shaders/shader.vs", "./src/shaders/shader.fs");

    // Create reference containers for the Vertex Array Object and the Vertex Buffer Object
    GLuint VAOs[1], VBOs[1], EBOs[1];

    // Generate VAO and VBO with only 1 object each
    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, VBOs);
    glGenBuffers(1, EBOs);

    // Make the VAO the current Vertex Array Object by binding it
    glBindVertexArray(VAOs[0]);

    // Bind the VBO specifying it's a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    // Introduce the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind the EBO specifying it's a GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    // Introduce the indices into the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // Enable the Vertex Attribute so that OpenGL knows to use it
    glEnableVertexAttribArray(0);

    // Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Main event loop
    while (!glfwWindowShouldClose(window))
    {
      // Specify clear values for the color buffers
      glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
      // Actually perform clearing of the buffers
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader->use();

      glBindVertexArray(VAOs[0]);

      // glDrawArrays(GL_TRIANGLES, 0, 3);
      glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

      // Swap the back buffer with the front buffer
      glfwSwapBuffers(window);

      // Handle all GLFW events
      glfwPollEvents();
    }

    glDeleteVertexArrays(1, VAOs);
    glDeleteBuffers(1, VBOs);
    glDeleteBuffers(1, EBOs);

    // Free shader object
    delete shader;

    // Destruct window prior to ending program
    glfwDestroyWindow(window);
    // Terminate GLFW prior to ending program
    glfwTerminate();
  }
  catch (string& e)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
    cerr << e << endl;

    // Free shader object
    delete shader;

    return EXIT_FAILURE;
#pragma GCC diagnostic pop
  }

  return EXIT_SUCCESS;
}