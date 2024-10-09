#include <GL/glew.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

// Vertex data for the triangle
GLfloat vertices[] = {
    0.0f,  0.5f,  0.0f, // Top vertex
    -0.5f, -0.5f, 0.0f, // Bottom-left vertex
    0.5f,  -0.5f, 0.0f  // Bottom-right vertex
};

// Shader sources
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout(location = 0) in vec3 position;\n"
                                 "out vec3 fragColor;\n"
                                 "void main() {\n"
                                 "    gl_Position = vec4(position, 1.0);\n"
                                 "    fragColor = vec3(0.5 + 0.5 * position.x, "
                                 "0.5 + 0.5 * position.y, 0.5);\n"
                                 "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
                                   "in vec3 fragColor;\n"
                                   "out vec4 color;\n"
                                   "void main() {\n"
                                   "    color = vec4(fragColor, 1.0);\n"
                                   "}\0";

// Function to compile shader
GLuint compileShader(GLenum type, const char *source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  return shader;
}

int main() {
  Display *display;
  Window window;
  GLXContext glc;
  XSetWindowAttributes swa;
  XVisualInfo *vi;
  Colormap cmap;
  int screen;

  // Connect to the X server
  display = XOpenDisplay(NULL);
  if (display == NULL) {
    printf("Cannot connect to X server\n");
    return 1;
  }

  screen = DefaultScreen(display);

  // Choose visual info
  static int attributes[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER,
                             None};
  vi = glXChooseVisual(display, screen, attributes);
  if (vi == NULL) {
    printf("No appropriate visual found\n");
    return 1;
  }

  // Create a colormap
  cmap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual,
                         AllocNone);
  swa.colormap = cmap;
  swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
                   ButtonReleaseMask | PointerMotionMask;

  // Create a window
  window = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0, 800,
                         600, 0, vi->depth, InputOutput, vi->visual,
                         CWColormap | CWEventMask, &swa);
  XMapWindow(display, window);
  XStoreName(display, window, "OpenGL X11 Rainbow Triangle");

  // Create an OpenGL context
  glc = glXCreateContext(display, vi, NULL, GL_TRUE);
  glXMakeCurrent(display, window, glc);

  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize GLEW\n");
    return -1;
  }

  // Set clear color
  glClearColor(0.0, 0.0, 0.0, 1.0);

  // Compile shaders
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  GLuint fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  // Create shader program and link shaders
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  // Event loop
  XEvent xev;
  while (1) {
    XNextEvent(display, &xev);

    if (xev.type == Expose) {
      // Render the rainbow triangle
      glClear(GL_COLOR_BUFFER_BIT);

      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, vertices);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glDisableClientState(GL_VERTEX_ARRAY);

      // Swap buffers
      glXSwapBuffers(display, window);
    }

    if (xev.type == KeyPress) {
      KeySym key = XLookupKeysym(&xev.xkey, 0);
      if (key == XK_Q || key == XK_q) {
        printf("Q pressed, exiting.\n");
        break; // Exit on 'Q' key press
      }
    }
  }

  // Clean up
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteProgram(shaderProgram);
  glXMakeCurrent(display, None, NULL);
  glXDestroyContext(display, glc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  return 0;
}
