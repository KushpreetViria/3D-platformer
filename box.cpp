#include "box.h"

Box::Box(float x, float y, float z, float w, glm::vec3 c, bool p) :
    width(w), color(c),pillar(p),pos(glm::vec3(x,y,z)) {
}

float Box::getTop()
{
    return pos.y - width/2;
}

float Box::getBot()
{
    return pos.y + width/ 2;
}

float Box::getLeft()
{
    return pos.x - width / 2;
}

float Box::getRight()
{
    return pos.x + width / 2;
}

float Box::getFront()
{
    return pos.z + width / 2;
}

float Box::getBack()
{
    return pos.z - width / 2;
}

Box::~Box()
{
}
