#include <GLFW/glfw3.h>
#include <iostream>
#include "functions/MainFunctions.hpp"

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // Remove this line: glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = createWindow(800, 800, "Flight Simulator");

    if (!window) return -1;

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);

    Model plane = loadObj("src/assets/plane/plane.obj");

    if (plane.vertices.empty()) {
        std::cerr << "Model is empty! Check if plane.obj exists." << std::endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2, 2, -2, 2, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);

        // Position the camera
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -5.0f);

        // Mouse Controls
        glRotatef(camera.rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(camera.rotationY, 0.0f, 1.0f, 0.0f);

        glRotatef(-85.0f, 1.0f, 0.0f, 1.0f);
        
        glColor3f(1.0f, 1.0f, 1.0f);
        
        renderModel(plane);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}