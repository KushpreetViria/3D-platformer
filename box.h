#pragma once
#include <glm/glm.hpp>

class Box {
public:
	float x;
	float y;
	float z;
	glm::vec3 color;
	
	float width;

	bool pillar;

	Box(float x, float y, float z, float w, glm::vec3 col, bool pillar);
	float getTop();
	float getBot();
	float getLeft();
	float getRight();
	float getFront();
	float getBack();

	~Box();
};
