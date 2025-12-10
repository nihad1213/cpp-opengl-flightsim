#include "MainFunctions.hpp"
#include <iostream>

/**
 * createWindow: Create window
 */
GLFWwindow* createWindow(int width, int height, const std::string &title) {
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    return window;
}

