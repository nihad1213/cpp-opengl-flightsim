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

    generateReferenceCubes();

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2, 2, -2, 2, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);

    double lastTime = glfwGetTime();

    std::cout << "\n=== Flight Simulator Controls ===" << std::endl;
    std::cout << "W/S: Increase/Decrease speed" << std::endl;
    std::cout << "A/D: Turn left/right (yaw)" << std::endl;
    std::cout << "Arrow Up/Down: Pitch up/down" << std::endl;
    std::cout << "Arrow Left/Right: Roll left/right" << std::endl;
    std::cout << "Mouse drag: Rotate view" << std::endl;
    std::cout << "R: Reset camera and plane" << std::endl;
    std::cout << "ESC: Exit" << std::endl;
    std::cout << "================================\n" << std::endl;

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;

        updatePlaneControls(window, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -5.0f);

        // Mouse Controls
        glRotatef(camera.rotationX, 1.0f, 0.0f, 0.0f);
        glRotatef(camera.rotationY, 0.0f, 1.0f, 0.0f);

        renderGroundGrid();

        renderReferenceCubes();

        glRotatef(planeState.rotX, 1.0f, 0.0f, 0.0f);
        glRotatef(planeState.rotY, 0.0f, 1.0f, 0.0f);
        glRotatef(planeState.rotZ, 0.0f, 0.0f, 1.0f);

        glRotatef(-85.0f, 1.0f, 0.0f, 1.0f);
        
        glColor3f(1.0f, 1.0f, 1.0f);
        
        renderModel(plane);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}