#ifndef MAIN_FUNCTIONS_HPP
#define MAIN_FUNCTIONS_HPP

#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <math.h>

GLFWwindow* createWindow(int width, int height, const std::string &title);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

struct Vertex {
    float x, y, z;
};

struct Face {
    int v1, v2, v3;
};

struct Model {
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
};

struct Camera {
    float rotationX;
    float rotationY;
    double lastMouseX;
    double lastMouseY;
    bool isDragging;
};

struct PlaneState {
    float posX, posY, posZ;
    float rotX, rotY, rotZ;
    float speed;
};

struct Cube {
    float x, y, z;
    float size;
    float r, g, b;
};

extern Camera camera;
extern PlaneState planeState;
extern std::vector<Cube> referenceCubes;

Model loadObj(const std::string &filepath);
void renderModel(const Model &model);
void updatePlaneControls(GLFWwindow* window, float deltaTime);
void generateReferenceCubes();
void renderCube(const Cube &cube);
void renderReferenceCubes();
void renderGroundGrid();

#endif