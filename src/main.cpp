#include <math.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "gfx/shader/shader.h"
#include "gfx/shader/VAO.h"
#include "gfx/shader/VBO.h"
#include "gfx/shader/EBO.h"

using namespace std;

#define APP_VERSION "0.1.1"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define TEXTURE_NR 16
#define TEXTURE_X1 (float) ((TEXTURE_NR % 16) * (float) 16/256)
#define TEXTURE_X2 TEXTURE_X1 + (float) 16/256

#define TEXTURE_Y2 (float) 1 - (TEXTURE_NR / 16 * (float) 16/256)
#define TEXTURE_Y1 TEXTURE_Y2 - (float) 16/256

// Vertices for the triangles
GLfloat vertices[] = {
  // Coordinates ------- Colors ------------ Texture coordinates
  -0.5f, -0.5f, 0.0f,    0.8f, 0.3f,  0.02f, TEXTURE_X1, TEXTURE_Y1,  // Lower left corner
  -0.5f,  0.5f, 0.0f,    1.0f, 0.6f,  0.32f, TEXTURE_X1, TEXTURE_Y2,  // Upper left corner
   0.5f,  0.5f, 0.0f,    1.0f, 0.6f,  0.32f, TEXTURE_X2, TEXTURE_Y2,  // Upper right corner
   0.5f, -0.5f, 0.0f,    0.8f, 0.3f,  0.02f, TEXTURE_X2, TEXTURE_Y1,  // Lower right corner
};

// Indices for the vertex ordering
GLuint indices[] = {
  0, 2, 1, // Lower triangle
  0, 3, 2, // Upper triangle
};

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
    cout << "OpenGL version supported: " << version << endl << "--------------" << endl;

    // Configure the viewport used by OpenGL in the window
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Specify clear values for the color buffers
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Actually perform clearing of the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);

    // Construct shader object
    shader = new Shader("./src/resources/shaders/shader.vs", "./src/resources/shaders/shader.fs");

    // Generate vertex array object and bind it
    VAO VAO1;
    VAO1.bind();

    // Generate vertex and element buffer objects
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // Links VBO to VAO
    VAO1.linkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.linkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.linkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Unbind all to prevent accidentally modifying them
    VAO1.unbind();
    VBO1.unbind();
    EBO1.unbind();

    // Create/define uniform 'scale' for use in shader
    GLuint uniID = glGetUniformLocation(shader->ID, "scale");

    // Texture
    int iImgWidth, iImgHeight, iNrColorChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("./src/resources/textures/blocks.png", &iImgWidth, &iImgHeight, &iNrColorChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Texture settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float flatColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iImgWidth, iImgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint tex0Uniform = glGetUniformLocation(shader->ID, "tex0");
    shader->activate();
    glUniform1i(tex0Uniform, 0);

    float fScale = 1.0f; // 0.0f;
    float tLast = 0.0f;

    // Main event loop
    while (!glfwWindowShouldClose(window))
    {
      // Specify clear values for the color buffers
      glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
      // Actually perform clearing of the buffers
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Tell OpenGL which Shader Program we want to use
      shader->activate();

      // if (glfwGetTime() - tLast > (1.0f / 60.0f)) {
      //   fScale += 0.05f;
      //   tLast = glfwGetTime();
      // }

      // Initialize uniform value 'scale'
      // glUniform1f(uniID, abs(0.1f + sin(fScale)));
      glUniform1f(uniID, fScale);

      glBindTexture(GL_TEXTURE_2D, texture);

      // Bind the VAO so OpenGL knows to use it
      VAO1.bind();

      // Draw primitives, number of indices, datatype of indices, index of indices
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      // Swap the back buffer with the front buffer
      glfwSwapBuffers(window);

      // Handle all GLFW events
      glfwPollEvents();
    }

    VAO1.remove();
    VBO1.remove();
    EBO1.remove();
    glDeleteTextures(1, &texture);

    // Free shader object
    shader->deactivate();

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