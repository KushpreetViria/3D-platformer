#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#include <iostream>

#include "player.h"

constexpr float PI = glm::pi<float>();
constexpr float TWO_PI = glm::pi<float>() * 2;

float mylerp(float t, float a, float b) {
	return (1 - t) * a + t * b;
}

float mylerpAcuteAngle(float t, float a, float b)
{
	bool reverse = (b < a);
	if (abs(b - a) > PI) {
		if (reverse)
			a -= TWO_PI;
		else
			b -= TWO_PI;
	}
	return mylerp(t, a, b);
}

Player::Player(float x, float y, float z, float width) : Box(x,y,z,width, glm::vec3(1, 0, 0),false), inJump(false),currJumpFrame(0),xVector(0.0f),zVector(0.0f)
	,lerpStepsLeft(0),lerpStepsRight(0),lerpStepsFront(0),lerpStepsBack(0),frame(0),rotationAngle(0),rotationAxis(glm::vec3(1,0,0)){}

void Player::startJump()
{
	if (inJump == false && inRotation == false) {
		inJump = true;
		inRotation = true;

		//determine direction of rotation and store it
		if (xVector > 0.0f)
			lastMoveDirection = boxSides::RIGHT;
		else if (xVector < 0.0f)
			lastMoveDirection = boxSides::LEFT;
		else if (zVector > 0.0f)
			lastMoveDirection = boxSides::FRONT;
		else
			lastMoveDirection = boxSides::BACK;
	}
}

void Player::update()
{
	frame ++;
	frame = frame % 60;

	//random color every 10 frames
	if (frame % 10 == 0) {
		color.r = glm::linearRand(0.0f, 1.0f);
		color.g = glm::linearRand(0.0f, 1.0f);
		color.b = glm::linearRand(0.0f, 1.0f);
	}

	//jump up and down in "maxJumpFrames" number of frames
	if (inJump && currJumpFrame < totalJumpFrames/2) {
		y += jumpSpeed;
		currJumpFrame++;
	}else if (inJump && currJumpFrame >= totalJumpFrames/2) {
		if (currJumpFrame >= totalJumpFrames) {
			inJump = false;
		}else {
			y -= jumpSpeed;
			currJumpFrame++;
		}

	}else {
		currJumpFrame = 0;
	}

	rotateBox();

	x += xVector;
	z += zVector;
}

float Player::getBoundary(boxSides side)
{
	float returnVal = 0.0f;
	
	if (side == boxSides::BOT)
		returnVal = getBot();
	else if (side == boxSides::TOP)
		returnVal = getTop();
	else if (side == boxSides::LEFT)
		returnVal = getLeft();
	else if (side == boxSides::RIGHT)
		returnVal = getRight();
	else if (side == boxSides::FRONT)
		returnVal = getFront();
	else 
		returnVal = getBack();

	return returnVal;
}

Player::~Player()
{	}

void Player::rotateBox()
{		
	if (inRotation) {
		//originally meant to rotate whichever way midair, hense the 4 different time counting variables... ended up doing just one directional rotation per jump however
		float tLeft = (float)lerpStepsLeft / (float)totalJumpFrames; // for flipping to left
		float tRight = (float)lerpStepsRight / (float)totalJumpFrames;  //for flipping to right
		float tFront = (float)lerpStepsFront / (float)totalJumpFrames; // for flipping towards front
		float tBack = (float)lerpStepsBack / (float)totalJumpFrames;  //for flipping towards back

		if (lastMoveDirection == boxSides::RIGHT)
			inRotation = rotateBy(inRotation, tRight, lerpStepsRight, -PI, glm::vec3(0, 0, 1.0f));
		else if (lastMoveDirection == boxSides::LEFT)
			inRotation = rotateBy(inRotation, tLeft, lerpStepsLeft, PI, glm::vec3(0, 0, 1.0f));
		else if (lastMoveDirection == boxSides::FRONT)
			inRotation = rotateBy(inRotation, tFront, lerpStepsFront, PI, glm::vec3(1.0f, 0, 0));
		else
			inRotation = rotateBy(inRotation, tBack, lerpStepsBack, -PI, glm::vec3(1.0f, 0, 0));
	}
}

bool Player::rotateBy(bool rotating, float time, int &steps, float angle, glm::vec3 axis) {
	if (rotating && time < 1.0f) {//&& xVector > 0) { //right
		float theta = mylerpAcuteAngle(time, 0, angle);
		rotationAxis = axis;
		rotationAngle = theta;
		steps = (steps + 1) % (totalJumpFrames+1);
		return true;
	}
	else if (time >= 1.0f) {
		steps = 0.0f;
		return false;
	}
}