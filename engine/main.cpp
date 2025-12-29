// main.cpp
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// NEW: Include for Emscripten
#include <emscripten/emscripten.h>

#include "math3d.h"
#include "model.h"
#include "renderer.h"

// NEW: Store all our persistent state in one place.
struct GlobalState {
    std::vector<Triangle> triangles;
    Vec3 lightDir;
    
    // Animation variables
    float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;
    const float rotationSpeed = 0.02f;
    
    // Buffers
    std::vector<std::string> buffer;
    std::vector<float> zbuffer;
};

// This becomes our new "main loop"
void main_loop(void* arg) {
    GlobalState* state = static_cast<GlobalState*>(arg);
    
    // Clear buffers
    clearBuffers(state->buffer, state->zbuffer);
    
    // Create rotation matrix
    Mat3 rotation = rotationX(state->angleX) * rotationY(state->angleY) * rotationZ(state->angleZ);
    
    // Render the frame
    renderFrame(state->buffer, state->zbuffer, state->triangles, rotation, state->lightDir);
    
    // Print the result (this function will be modified next)
    printBuffer(state->buffer);
    
    // Update rotation angles
    state->angleX += state->rotationSpeed;
    state->angleY += state->rotationSpeed * 1.3f;
    state->angleZ += state->rotationSpeed * 0.7f;
}

// main() is now just for initialization.
int main(int argc, char* argv[]) {
    // We'll use Emscripten's virtual filesystem.
    // We expect the JS host to place the file at "/model.stl"
    const char* filename = (argc > 1) ? argv[1] : "/model.stl";
    
    GlobalState* state = new GlobalState();
    
    // Load STL file
    state->triangles = loadSTL(filename);
    if (state->triangles.empty()) {
        std::cerr << "Failed to load model or model is empty." << std::endl;
        return 1;
    }
    
    // Normalize model
    float modelScale;
    normalizeModel(state->triangles, modelScale);
    
    // Light direction
    state->lightDir = Vec3(0.5f, -0.7f, -0.5f).normalize();
    
    // Initialize buffers
    state->buffer.resize(SCREEN_HEIGHT, std::string(SCREEN_WIDTH, ' '));
    state->zbuffer.resize(SCREEN_WIDTH * SCREEN_HEIGHT);
    
    std::cout << "Starting renderer..." << std::endl;
    
    // Tell Emscripten to call main_loop() forever.
    // -1 = use browser's requestAnimationFrame()
    // 1 = simulate infinite loop
    emscripten_set_main_loop_arg(main_loop, state, 30, 1);    
    return 0;
}