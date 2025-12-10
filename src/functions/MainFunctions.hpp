#ifndef MAIN_FUNCTIONS_HPP
#define MAIN_FUNCTIONS_HPP

#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <math.h>

GLFWwindow* createWindow(int width, int height, const std::string &title);

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

Model loadObj(const std::string &filepath);
void renderModel(const Model &model);


#endif
