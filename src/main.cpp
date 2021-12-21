#include <math.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gfx/shader/shader.h"
#include "gfx/texture/texture.h"
#include "gfx/shader/VAO.h"
#include "gfx/shader/VBO.h"
#include "gfx/shader/EBO.h"

#include "gfx/camera/camera.h"

using namespace std;

#define APP_VERSION "0.1.1"

#define TEXTURE_NR 1
#define TEXTURE_X1 (float) ((TEXTURE_NR % 16) * (float) 16/256)
#define TEXTURE_X2 TEXTURE_X1 + (float) 16/256

#define TEXTURE_Y2 (float) 1 - (TEXTURE_NR / 16 * (float) 16/256)
#define TEXTURE_Y1 TEXTURE_Y2 - (float) 16/256

const unsigned int uiScreenWidth = 800;
const unsigned int uiScreenHeight = 800;

// Vertices for the triangles
GLfloat vertices[] = {
  // Coordinates ------- Colors ----------- Texture coordinates
  -0.5f, -0.5f, -0.5f,    0.8f, 0.3f, 0.02f, TEXTURE_X1, TEXTURE_Y1,  // Front lower left corner  0 
  -0.5f,  0.5f, -0.5f,    1.0f, 0.6f, 0.32f, TEXTURE_X1, TEXTURE_Y2,  // Front upper left corner  1
   0.5f,  0.5f, -0.5f,    1.0f, 0.6f, 0.32f, TEXTURE_X2, TEXTURE_Y2,  // Front upper right corner 2
   0.5f, -0.5f, -0.5f,    0.8f, 0.3f, 0.02f, TEXTURE_X2, TEXTURE_Y1,  // Front lower right corner 3

  -0.5f, -0.5f,  0.5f,    0.8f, 0.3f, 0.02f, TEXTURE_X1, TEXTURE_Y1,  // Back lower left corner   4
  -0.5f,  0.5f,  0.5f,    1.0f, 0.6f, 0.32f, TEXTURE_X1, TEXTURE_Y2,  // Back upper left corner   5
   0.5f,  0.5f,  0.5f,    1.0f, 0.6f, 0.32f, TEXTURE_X2, TEXTURE_Y2,  // Back upper right corner  6
   0.5f, -0.5f,  0.5f,    0.8f, 0.3f, 0.02f, TEXTURE_X2, TEXTURE_Y1,  // Back lower right corner  7
};

// Indices for the vertex ordering
GLuint indices[] = {
  0, 2, 1, // Front face - lower triangle
  0, 3, 2, // Front face - Upper triangle

  1, 5, 6, // Top face
  1, 2, 6, // Top face

  0, 3, 7, // Bottom face
  0, 4, 7, // Bottom face

  2, 3, 6, // Right face
  3, 6, 7, // Right face

  0, 4, 5, // Left face
  0, 1, 5, // Left face

  4, 5, 6, // Back face
  4, 6, 7, // Back face
};

int main(int argc, char* argv[])
{
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
    GLFWwindow* window = glfwCreateWindow(uiScreenWidth, uiScreenHeight, "Mynecraft", NULL, NULL);
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
    glViewport(0, 0, uiScreenWidth, uiScreenHeight);

    // Specify clear values for the color buffers
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Actually perform clearing of the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);

    // Construct shader object
    Shader shader("./src/resources/shaders/shader.vs", "./src/resources/shaders/shader.fs");

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
    GLuint uniID = glGetUniformLocation(shader.ID, "scale");

    // Textures
    Texture texture("./src/resources/textures/blocks.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    texture.texUnit(shader, "tex0", 0);

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);

    // Camera
    Camera camera(uiScreenWidth, uiScreenHeight, glm::vec3(0.0f, 0.0f, 2.0f));

    double lasttime = glfwGetTime();

    // Main event loop
    while (!glfwWindowShouldClose(window))
    {
      // Specify clear values for the color buffers
      glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
      // Actually perform clearing of the buffers
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Tell OpenGL which Shader Program we want to use
      shader.activate();

      camera.Inputs(window);
      camera.Matrix(90.0f, 0.1f, 100.0f, shader, "camMatrix");

      // Binds texture so that is appears in rendering
      texture.bind();

      // Bind the VAO so OpenGL knows to use it
      VAO1.bind();

      // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      // Draw primitives, number of indices, datatype of indices, index of indices
      glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

      // Swap the back buffer with the front buffer
      glfwSwapBuffers(window);

      // Handle all GLFW events
      glfwPollEvents();

      while (glfwGetTime() < lasttime + 1.0 / 60) {
        // TODO: Put the thread to sleep, yield, or simply do nothing
      }
      lasttime += 1.0 / 60;
    }

    VAO1.remove();
    VBO1.remove();
    EBO1.remove();
    // texture.remove();

    // Free shader object
    shader.deactivate();

    // Destruct window prior to ending program
    glfwDestroyWindow(window);
    // Terminate GLFW prior to ending program
    glfwTerminate();
  }
  catch (string& e)
  {
    cerr << e << endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}