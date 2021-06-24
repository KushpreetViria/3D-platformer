#pragma once
#include <glm/glm.hpp>
#include "box.h"

enum class boxSides {
	TOP,
	BOT,
	LEFT,
	RIGHT,
	FRONT,
	BACK
};

class Player : public Box{
public:
	glm::vec3 rotationAxis;
	float rotationAngle;

	float xVector;	
	float zVector;

	//default
	int totalJumpFrames = 60;
	float jumpSpeed = 0.01f;
	
	Player(float x, float y, float z, float width);
	void startJump();
	void update();
	float getBoundary(boxSides side);
	~Player();

private:
	int frame;
	int currJumpFrame;

	bool inJump;
	bool inRotation = false;
	
	int lerpStepsLeft;
	int lerpStepsRight;
	int lerpStepsBack;
	int lerpStepsFront;

	boxSides lastMoveDirection = boxSides::FRONT; //default

	void rotateBox();
	bool rotateBy(bool rotating, float time, int& steps, float angle, glm::vec3 axis);

};