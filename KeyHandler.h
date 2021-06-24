#pragma once
#include <string>
#include <GLFW/glfw3.h>

const int KEY_JUMP = GLFW_KEY_SPACE;
const int KEY_TEX = GLFW_KEY_T;
const int KEY_COLLISION = GLFW_KEY_C;
const int DEBUG = GLFW_KEY_ENTER;

bool escKey = false;
bool keyLeft = false;;
bool keyRight = false;;
bool keyBack = false;;
bool keyForward = false;;
bool keyJump = false;;

bool doTextures = false;;
bool doCollision = false;;
bool debug = false;;

std::string printBool(bool input) {
    return input ? "TRUE" : "FALSE";
}

void key_press_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE)
            escKey = !escKey;

        if (key == GLFW_KEY_W)
            keyForward = true;
        if (key == GLFW_KEY_S)
            keyBack = true;
        if (key == GLFW_KEY_A)
            keyLeft = true;
        if (key == GLFW_KEY_D)
            keyRight = true;
        if (key == KEY_JUMP)
            keyJump = true;

        if (key == KEY_TEX)
            doTextures = !doTextures;
        if (key == KEY_COLLISION)
            doCollision = !doCollision;
        if (key == DEBUG)
            debug = !debug;

    }
    else if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_W)
            keyForward = false;
        if (key == GLFW_KEY_S)
            keyBack = false;
        if (key == GLFW_KEY_A)
            keyLeft = false;
        if (key == GLFW_KEY_D)
            keyRight = false;
        if (key == KEY_JUMP)
            keyJump = false;
    }
    
    if (debug) {
        std::cout << "Collisions: " << printBool(doCollision) << std::endl;
        std::cout << "Textures: " << printBool(doTextures) << std::endl;
        std::cout << "Debug: " << printBool(debug) << std::endl;
    }
}