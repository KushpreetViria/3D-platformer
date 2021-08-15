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
	Player(float x, float y, float z, float width, glm::vec3 worldUp = glm::vec3(0.0f,1.0f,0.0f),glm::vec3 front = glm::vec3(1.0f,0.0f,0.0f));
	
	void setDirection(boxSides);
	void setHorizontalSpeed(float);
	void setVerticalSpeed(float);
	
	float getRotationAngle();
	glm::vec3 getRotationAxis();
	glm::vec3 getFrontVec();

	void startJump();
	void update(float);
	float getBoundary(boxSides side);
	~Player();

private:
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

	//variables for lerping
	int frame = 0;
	int currJumpFrame = 0;
	int lerpSteps = 0;

	bool inJump = false;
	bool inRotation = false;

	// player directional and speed state
	boxSides myDirection;
	float myXZSpeed;
	float myYSpeed;

	void movePlayer(float speed, float deltaTime, boxSides direction);
	void rotateBox();
	bool rotateBy(bool rotating, float time, int& steps, float angle, glm::vec3 axis);
	void updateVectors();

};