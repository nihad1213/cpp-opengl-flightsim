#include "MainFunctions.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

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

/**
 * loadObj: read .obj files
 */
Model loadObj(const std::string &filepath) {
    Model model;
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Cannot open: " << filepath << std::endl;
        return model;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        
        if (type == "v") {
            Vertex v;
            iss >> v.x >> v.y >> v.z;
            model.vertices.push_back(v);
        } else if (type=="f") {
            Face f;
            std::string v1, v2, v3;
            iss >> v1 >> v2 >> v3;
            
            f.v1 = std::stoi(v1.substr(0, v1.find('/'))) - 1;
            f.v2 = std::stoi(v2.substr(0, v2.find('/'))) - 1;
            f.v3 = std::stoi(v3.substr(0, v3.find('/'))) - 1;
            
            model.faces.push_back(f);
        }
    }

    file.close();
    std::cout << "Loaded: " << model.vertices.size() << " vertices, " 
              << model.faces.size() << " triangles" << std::endl;
    
    return model;

}

/**
 * renderModel: draw model
 */
void renderModel(const Model &model) {
    glBegin(GL_TRIANGLES);

    for (const Face &face : model.faces) {
        const Vertex &v1 = model.vertices[face.v1];
        const Vertex &v2 = model.vertices[face.v2];
        const Vertex &v3 = model.vertices[face.v3];

        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
    }

    glEnd();
}

