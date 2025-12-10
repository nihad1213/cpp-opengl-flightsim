#include "MainFunctions.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <ctime>

// Camera and plane initialization
Camera camera = {20.0f, 0.0f, 0.0, 0.0, false};
PlaneState planeState = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
std::vector<Cube> referenceCubes;

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
 * keyCallback: Press ESC to close window, R to reset camera
 */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        camera.rotationX = 20.0f;
        camera.rotationY = 0.0f;
        planeState.posX = 0.0f;
        planeState.posY = 0.0f;
        planeState.posZ = 0.0f;
        planeState.rotX = 0.0f;
        planeState.rotY = 0.0f;
        planeState.rotZ = 0.0f;
        planeState.speed = 0.0f;
        std::cout << "Camera and plane reset" << std::endl;
    }
}

/**
 * mouseButtonCallback: Detect when mouse button is pressed/released
 */
void mouseButtonCallback(GLFWwindow* window, int button, int action, int /*mods*/) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            camera.isDragging = true;
            glfwGetCursorPos(window, &camera.lastMouseX, &camera.lastMouseY);
        } else if (action == GLFW_RELEASE) {
            camera.isDragging = false;
        }
    }
}

/**
 * cursorPosCallback: Track mouse movement
 */
void cursorPosCallback(GLFWwindow* /*window*/, double xpos, double ypos) {
    if (camera.isDragging) {
        double deltaX = xpos - camera.lastMouseX;
        double deltaY = ypos - camera.lastMouseY;
        
        camera.rotationY += static_cast<float>(deltaX) * 0.5f;
        camera.rotationX += static_cast<float>(deltaY) * 0.5f;
        
        if (camera.rotationX > 89.0f) camera.rotationX = 89.0f;
        if (camera.rotationX < -89.0f) camera.rotationX = -89.0f;
        
        camera.lastMouseX = xpos;
        camera.lastMouseY = ypos;
    }
}

/**
 * updatePlaneControls: Handle WASD and arrow key controls with REAL flight physics
 */
void updatePlaneControls(GLFWwindow* window, float deltaTime) {
    float acceleration = 3.0f * deltaTime;
    float turnSpeed = 80.0f * deltaTime;
    float maxSpeed = 8.0f;
    
    // W/S for throttle control
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        planeState.speed += acceleration;
        if (planeState.speed > maxSpeed) planeState.speed = maxSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        planeState.speed -= acceleration * 0.5f;
        if (planeState.speed < 0.0f) planeState.speed = 0.0f;
    }
    
    planeState.speed *= 0.995f;
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        planeState.rotX += turnSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        planeState.rotX -= turnSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        planeState.rotZ += turnSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        planeState.rotZ -= turnSpeed;
    }
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        planeState.rotY += turnSpeed * 0.8f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        planeState.rotY -= turnSpeed * 0.8f;
    }
    
    if (planeState.rotX > 60.0f) planeState.rotX = 60.0f;
    if (planeState.rotX < -60.0f) planeState.rotX = -60.0f;
    if (planeState.rotZ > 60.0f) planeState.rotZ = 60.0f;
    if (planeState.rotZ < -60.0f) planeState.rotZ = -60.0f;
    
    float pitchRad = planeState.rotX * M_PI / 180.0f;
    float yawRad = planeState.rotY * M_PI / 180.0f;
    
    float forwardX = sin(yawRad) * cos(pitchRad);
    float forwardY = -sin(pitchRad);
    float forwardZ = -cos(yawRad) * cos(pitchRad);
    
    planeState.posX += forwardX * planeState.speed * deltaTime;
    planeState.posY += forwardY * planeState.speed * deltaTime;
    planeState.posZ += forwardZ * planeState.speed * deltaTime;
}

/**
 * generateReferenceCubes: Create random cubes in space
 */
void generateReferenceCubes() {
    srand(static_cast<unsigned>(time(0)));
    referenceCubes.clear();
    
    for (int i = 0; i < 80; i++) {
        Cube cube;
        cube.x = (rand() % 200 - 100) * 0.5f;
        cube.y = (rand() % 40 + 2) * 0.3f;
        cube.z = (rand() % 200 - 100) * 0.5f;
        cube.size = 0.3f + (rand() % 15) * 0.1f;
        
        // More colorful cubes
        cube.r = 0.4f + (rand() % 60) * 0.01f;
        cube.g = 0.4f + (rand() % 60) * 0.01f;
        cube.b = 0.4f + (rand() % 60) * 0.01f;
        referenceCubes.push_back(cube);
    }
    
    std::cout << "Generated " << referenceCubes.size() << " reference cubes" << std::endl;
}

/**
 * renderGroundGrid: Draw a grid on the ground for spatial reference
 */
void renderGroundGrid() {
    glColor3f(0.3f, 0.4f, 0.3f);
    glBegin(GL_LINES);
    
    float gridSize = 100.0f;
    float gridStep = 5.0f;
    float groundY = -2.0f;
    
    for (float z = -gridSize; z <= gridSize; z += gridStep) {
        float relZ = z - planeState.posZ;
        glVertex3f(-gridSize - planeState.posX, groundY - planeState.posY, relZ);
        glVertex3f(gridSize - planeState.posX, groundY - planeState.posY, relZ);
    }
    
    for (float x = -gridSize; x <= gridSize; x += gridStep) {
        float relX = x - planeState.posX;
        glVertex3f(relX, groundY - planeState.posY, -gridSize - planeState.posZ);
        glVertex3f(relX, groundY - planeState.posY, gridSize - planeState.posZ);
    }
    
    glEnd();
}

/**
 * renderCube: Draw a single cube
 */
void renderCube(const Cube &cube) {
    float s = cube.size / 2.0f;
    
    glColor3f(cube.r, cube.g, cube.b);
    
    glBegin(GL_QUADS);
    
    // Front face
    glVertex3f(cube.x - s, cube.y - s, cube.z + s);
    glVertex3f(cube.x + s, cube.y - s, cube.z + s);
    glVertex3f(cube.x + s, cube.y + s, cube.z + s);
    glVertex3f(cube.x - s, cube.y + s, cube.z + s);
    
    // Back face
    glVertex3f(cube.x - s, cube.y - s, cube.z - s);
    glVertex3f(cube.x - s, cube.y + s, cube.z - s);
    glVertex3f(cube.x + s, cube.y + s, cube.z - s);
    glVertex3f(cube.x + s, cube.y - s, cube.z - s);
    
    // Top face
    glVertex3f(cube.x - s, cube.y + s, cube.z - s);
    glVertex3f(cube.x - s, cube.y + s, cube.z + s);
    glVertex3f(cube.x + s, cube.y + s, cube.z + s);
    glVertex3f(cube.x + s, cube.y + s, cube.z - s);
    
    // Bottom face
    glVertex3f(cube.x - s, cube.y - s, cube.z - s);
    glVertex3f(cube.x + s, cube.y - s, cube.z - s);
    glVertex3f(cube.x + s, cube.y - s, cube.z + s);
    glVertex3f(cube.x - s, cube.y - s, cube.z + s);
    
    // Right face
    glVertex3f(cube.x + s, cube.y - s, cube.z - s);
    glVertex3f(cube.x + s, cube.y + s, cube.z - s);
    glVertex3f(cube.x + s, cube.y + s, cube.z + s);
    glVertex3f(cube.x + s, cube.y - s, cube.z + s);
    
    // Left face
    glVertex3f(cube.x - s, cube.y - s, cube.z - s);
    glVertex3f(cube.x - s, cube.y - s, cube.z + s);
    glVertex3f(cube.x - s, cube.y + s, cube.z + s);
    glVertex3f(cube.x - s, cube.y + s, cube.z - s);
    
    glEnd();
}

/**
 * renderReferenceCubes: Draw all reference cubes relative to plane position
 */
void renderReferenceCubes() {
    for (const Cube &cube : referenceCubes) {
        Cube relativeCube = cube;
        relativeCube.x -= planeState.posX;
        relativeCube.y -= planeState.posY;
        relativeCube.z -= planeState.posZ;
        renderCube(relativeCube);
    }
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
    
    if (!model.vertices.empty()) {
        float minX = model.vertices[0].x, maxX = model.vertices[0].x;
        float minY = model.vertices[0].y, maxY = model.vertices[0].y;
        float minZ = model.vertices[0].z, maxZ = model.vertices[0].z;
        
        for (const Vertex &v : model.vertices) {
            if (v.x < minX) minX = v.x;
            if (v.x > maxX) maxX = v.x;
            if (v.y < minY) minY = v.y;
            if (v.y > maxY) maxY = v.y;
            if (v.z < minZ) minZ = v.z;
            if (v.z > maxZ) maxZ = v.z;
        }
        
        float centerX = (minX + maxX) / 2.0f;
        float centerY = (minY + maxY) / 2.0f;
        float centerZ = (minZ + maxZ) / 2.0f;
        
        float sizeX = maxX - minX;
        float sizeY = maxY - minY;
        float sizeZ = maxZ - minZ;
        float maxSize = std::max({sizeX, sizeY, sizeZ});
        
        float scale = 2.0f / maxSize;
        
        std::cout << "Model size: " << sizeX << " x " << sizeY << " x " << sizeZ << std::endl;
        std::cout << "Scaling by: " << scale << std::endl;
        
        for (Vertex &v : model.vertices) {
            v.x = (v.x - centerX) * scale;
            v.y = (v.y - centerY) * scale;
            v.z = (v.z - centerZ) * scale;
        }
        
        std::cout << "Model normalized and centered" << std::endl;
    }
    
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