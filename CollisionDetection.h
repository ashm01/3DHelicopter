#pragma once
#include <windows.h>		// Header File For Windows

#include <iostream>

#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
class CollisionDetection
{
public:
	CollisionDetection();
	bool sphereAABB(glm::vec3 spherePos, float sphereRad, glm::vec3 boxPos, glm::vec3 boxDimesions);
	bool sphereCheck(glm::vec3 aPos, double aRad, glm::vec3 bPos, double bRad);
};

