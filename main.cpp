#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <iostream>
#include <vector>

#include "shader.h"
#include "camera.h"
#include "box.h"
#include "KeyHandler.h"
#include "player.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void loadText(GLuint*, const char*, bool);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void createWorld();

const float xMinimum = -1.0f;
const float xMaximum = 5.0f;
const float zMinimum = 0.5f;
const float zMaximum = 1.0f;
const float offset = 0.05f;
const float speed = 0.5f;

// screen size settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//mouse position
bool firstMouse = true; 
float lastX;
float lastY;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

std::vector<Box*> worldArray;
Camera cam; //free-camera class
Player* player;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    cam = Camera(glm::vec3(0.0f, 0.5f, 2.0f)); //defualt    

    //set callback functions for event listeners    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_press_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program   
    Shader ourShader("Resources/Shaders/shader-vertex.glsl", "Resources/Shaders/shader-frag.glsl"); // you can name your shader files however you like

    //----------vertex data and bind vertex arrays--------------
    float vertices[] = {
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,

    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,

    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

     1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
    };

    float topVertices[] = {
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f, 1.0f
    };

    unsigned int VBO[2], VAO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //bind top vertices
    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(topVertices), topVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //------------load textures------------
    GLuint texture1;
    loadText(&texture1, "Resources/Textures/grassTex.jpg", false);
    GLuint texture2;
    loadText(&texture2, "Resources/Textures/stoneTex.jpg", false);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // set mouse coord to screen center
    lastX = SCR_WIDTH / 2.0f;
    lastY = SCR_HEIGHT / 2.0f;

    createWorld(); //generate the world coordinates to draw 
    cam = Camera(glm::vec3(player->pos.x,player->pos.y,player->pos.z+2.0f));

    // render loop
    while (!glfwWindowShouldClose(window))
    {      
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);        
        player->update();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();        

        //create transformations and send them to shaders
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        ourShader.setMat4("model", model);

        glm::mat4 view = cam.GetViewMatrix();
        ourShader.setMat4("view", view);

        glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f);
        ourShader.setMat4("projection", projection);

        //rotating camera
        //const float radius = 10.0f;
        //float camX = (float)sin(glfwGetTime()) * radius;
        //float camZ = (float)cos(glfwGetTime()) * radius;
        //glm::mat4 view;
        //view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

        ourShader.setBool("doText", doTextures);
        glActiveTexture(GL_TEXTURE0);

        //draw the world
        // scale = offset/2 because the generated world has boxes with width of offset=0.05, we need half of this which is offset/2=0.025 to allign the boxes properly.
        for (int i = 0; i < worldArray.size(); i++) {
            Box* b = worldArray[i];
            if (b != NULL) {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(b->pos.x, b->pos.y, b->pos.z + 0.01));
                model = glm::scale(model, glm::vec3(offset/2, offset / 2, offset / 2));
                ourShader.setMat4("model", model);
                ourShader.setVec3f("boxColor", b->color);
                
                //draw the sides + bottom of box
                glBindVertexArray(VAO[0]);
                glBindTexture(GL_TEXTURE_2D, texture2);
                glDrawArrays(GL_TRIANGLES, 0, 30);

                //draw the top part of the box with different texture
                glBindVertexArray(VAO[1]);                
                glBindTexture(GL_TEXTURE_2D, texture1);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }
        
        //draw player        
        ourShader.setBool("doText", false); //no textures for player
        
        model = glm::translate(glm::mat4(1.0f), glm::vec3(player->pos.x, player->pos.y, player->pos.z + 0.01));
        model = glm::rotate(model, player->rotationAngle, player->rotationAxis);
        model = glm::scale(model, glm::vec3(offset / 2, offset / 2, offset / 2));        
        ourShader.setMat4("model", model);
        ourShader.setVec3f("boxColor", player->color);

        glBindVertexArray(VAO[0]);        
        glDrawArrays(GL_TRIANGLES, 0, 30);

        glBindVertexArray(VAO[1]);            
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (debug) {
        if (firstMouse) {
            lastX = (float)xpos;
            lastY = (float)ypos;
            firstMouse = false;
        }

        float xoffset = (float)(xpos - lastX);
        float yoffset = (float)(lastY - ypos);
        lastX = (float)xpos;
        lastY = (float)ypos;

        cam.ProcessMouseMovement(xoffset, yoffset, true);
    }
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cam.ProcessMouseScroll((float)yoffset);
}
void loadText(GLuint* texture, const char* textFile, bool png) {
    glGenTextures(1, texture); //need to create an id for the texture
    glBindTexture(GL_TEXTURE_2D, *texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

    //wraping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load image with mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(textFile, &width, &height, &nrChannels, 0);
    if (data) {
        if (!png)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}


//do this whole world generation again
void createWorld() {
    int curr = 0;

    int xTotal = (int)((xMaximum - xMinimum) / offset);
    int zTotal = (int)((zMaximum - zMinimum) / offset);
    long size = xTotal * zTotal;
    
    worldArray.reserve(size);

    for (int currX = 0; currX < xTotal; currX++) {
        for (int currZ = 0; currZ < zTotal; currZ++) {
            float x = (float)((currX * offset) + xMinimum);
            float z = (float)(currZ * offset) + zMinimum;
            glm::vec3 randCol = glm::vec3(glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f), glm::linearRand(0.0f, 1.0f));

            if (currX == (int)(xTotal / 2) && currZ == (int)(zTotal / 2)) { //this center box is for spawning player, dont want pillar here
                Box* b = new Box(x, 0, z, offset, randCol, false);
                worldArray[curr++] = b;
            }
            else { //other wise create a box, but with a small chance of creating a pillar
                int rand = (int)(glm::linearRand(0, 100));
                if (rand >= 5) {  //most (95%) of the time, make a small box
                    Box* b = new Box(x, 0, z, offset, randCol, false);
                    worldArray.push_back(b);
                }
                else {  //small percent of the time, make a pillar
                    int height = (int)glm::linearRand(1, 4);
                    for (int h = 0; h < height+1; h++) {
                        Box* b = new Box(x, 0 + h*(offset), z, offset, randCol, true); //mark it as a pillar for easy collision checking later
                        worldArray.push_back(b); //vector will need to resize to account for these addional boxes
                    }                    
                }
            }     
        }       
    }
    player = new Player((xMaximum + xMinimum) / 2, 0+offset, (zMaximum + zMinimum) / 2,offset); //spwan player in the center
}

// process all input: whether relevant keys are pressed/released this frame and react 
void processInput(GLFWwindow* window)
{ 
    if (escKey)
        glfwSetWindowShouldClose(window, true);

    if (debug){ 
        Camera_Movement direction = Camera_Movement::NONE;
        if (keyForward)
            direction = Camera_Movement::FORWARD;
        if (keyBack)
            direction = Camera_Movement::BACKWARD;
        if (keyLeft)
            direction = Camera_Movement::LEFT;
        if (keyRight)
            direction = Camera_Movement::RIGHT;
        if (keyJump)
            direction = Camera_Movement::UP;

        cam.ProcessKeyboard(direction, deltaTime);
    
    }else {
        boxSides direction = boxSides::NONE;
        if (keyForward) {
            direction = boxSides::FRONT;
        }
        if (keyBack) {
            direction = boxSides::BACK;
        }
        if (keyLeft) {           
            direction = boxSides::LEFT;
        }
        if (keyRight) {            
            direction = boxSides::RIGHT;
        }

        if (keyJump)
            player->startJump();
        
         cam.Front = player->front;
         cam.Position = glm::vec3(player->pos.x - 1.0f, player->pos.y + 0.3f, player->pos.z);
         
         player->movePlayer(speed, deltaTime, direction);
    }

    //bound check
    if (player->getBoundary(boxSides::BOT) < 0.0f)
        player->pos.y = offset;

    if (player->pos.x< xMinimum) {
        player->pos.x = xMinimum;
    }
    if (player->pos.x> xMaximum - offset) {
        player->pos.x = xMaximum - offset;
    }
    if (player->pos.z > zMaximum - offset) {
        player->pos.z = zMaximum - offset;
    }
    if (player->pos.z < zMinimum) {
        player->pos.z = zMinimum;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}