#include "box.h"

Box::Box(float x_val, float y_val, float z_val, float w, glm::vec3 c, bool p) :
    x(x_val), y(y_val), z(z_val), width(w), color(c),pillar(p) {
}

float Box::getTop()
{
    return y - width/2;
}

float Box::getBot()
{
    return y + width/ 2;
}

float Box::getLeft()
{
    return x - width / 2;
}

float Box::getRight()
{
    return x + width / 2;
}

float Box::getFront()
{
    return z + width / 2;
}

float Box::getBack()
{
    return z - width / 2;
}

Box::~Box()
{
}
