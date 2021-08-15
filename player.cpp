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

glm::vec3 front;
glm::vec3 up;
glm::vec3 right;
glm::vec3 worldUp;

Player::Player(float x, float y, float z, float width,glm::vec3 worldUp, glm::vec3 front) : 
	Box(x,y,z,width, glm::vec3(1, 0, 0),false), inJump(false),currJumpFrame(0), lerpSteps(0),
	frame(0),rotationAngle(0),rotationAxis(glm::vec3(1,0,0)),worldUp(worldUp),front(front){

	updateVectors();

	std::cout << "pos: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
	std::cout << "right: " << right.x << ", " << right.y << ", " << right.z << std::endl;
	std::cout << "up: " << up.x << ", " << up.y << ", " << up.z << std::endl;
	std::cout << "front: " << front.x << ", " << front.y << ", " << front.z << std::endl;
}

void Player::setDirection(boxSides side){
	this->myDirection = side;
}

void Player::setHorizontalSpeed(float speed)
{
	this->myXZSpeed = speed;
}

void Player::setVerticalSpeed(float speed)
{
	this->myYSpeed = speed;
}

float Player:: getRotationAngle(){
	return this->rotationAngle;
}

glm::vec3 Player::getRotationAxis()
{
	return this->rotationAxis;
}

glm::vec3 Player::getFrontVec()
{
	return this->front;
}

void Player::startJump()
{
	if (inJump == false && inRotation == false) {
		inJump = true;
		inRotation = true;
	}
}

void Player::update(float deltaTime)
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
	if (inJump && currJumpFrame < int(totalJumpFrames/2)) {
		pos.y += myYSpeed * deltaTime;
		currJumpFrame++;
	}else if (inJump && currJumpFrame >= int(totalJumpFrames/2)) {
		if (currJumpFrame >= totalJumpFrames) {
			inJump = false;
		}else {
			pos.y -= myYSpeed* deltaTime;
			currJumpFrame++;
		}

	}else {
		currJumpFrame = 0;
	}
	movePlayer(myXZSpeed, deltaTime, myDirection);
	rotateBox();	
}

void Player::movePlayer(float speed, float deltaTime, boxSides direction) {
	float velocity = speed * deltaTime;
	if (direction == boxSides::FRONT) {
		pos += front * velocity;
	} else if (direction == boxSides::BACK) {
		pos -= front * velocity;
	}else if (direction == boxSides::RIGHT) {
		pos += right * velocity;
	}else if (direction == boxSides::LEFT) {
		pos -= right * velocity;
	}
	updateVectors();
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

void Player::rotateBox()
{		
	if (inRotation) {
		float time = (float)lerpSteps / (float)totalJumpFrames;	
		inRotation = rotateBy(inRotation, time, lerpSteps, -PI, glm::vec3(0, 0, 1.0f));
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
		steps = 0;
		return false;
	}	
	return false;
}

void Player::updateVectors() {	
	front = glm::normalize(front);	
	right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}

Player::~Player()
{	}