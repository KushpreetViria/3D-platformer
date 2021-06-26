#pragma once
#include <glm/glm.hpp>
#include "box.h"

enum class boxSides {
	TOP,
	BOT,
	LEFT,
	RIGHT,
	FRONT,
	BACK,
	NONE
};

class Player : public Box{
public:
	//player directional vectors
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	//for jumping
	glm::vec3 rotationAxis;
	float rotationAngle;

	//default
	int totalJumpFrames = 60;
	float jumpSpeed = 0.01f;
	
	Player(float x, float y, float z, float width, glm::vec3 worldUp = glm::vec3(0.0f,1.0f,0.0f),glm::vec3 front = glm::vec3(1.0f,0.0f,0.0f));
	void startJump();
	void update();
	void movePlayer(float speed, float deltaTime, boxSides direction);
	float getBoundary(boxSides side);
	~Player();

private:
	int frame;
	int currJumpFrame;

	bool inJump;
	bool inRotation = false;
	int lerpSteps;	

	void rotateBox();
	bool rotateBy(bool rotating, float time, int& steps, float angle, glm::vec3 axis);
	
	void updateVectors();

};