#include "CollisionDetection.h"
#include <iostream>

using namespace::std;

CollisionDetection::CollisionDetection()
{

}

bool CollisionDetection::sphereCheck(glm::vec3 aPos, double aRad, glm::vec3 bPos,
	double bRad)
{
	double xSquared = aPos.x - bPos.x;
	xSquared *= xSquared;
	double ySquared = aPos.y - bPos.y;
	ySquared *= ySquared;
	double zSquared = aPos.z - bPos.z;
	zSquared *= zSquared;

	double sumRad = aRad + bRad;
	sumRad *= sumRad;

	if (xSquared + ySquared + zSquared<= sumRad)
	{
		return true;
	}

	return false;
}

bool CollisionDetection::sphereAABB(glm::vec3 spherePos, float sphereRad, glm::vec3 boxPos, glm::vec3 boxDimesions)
{
	// Get the center of the sphere relative to the center of the box
	glm::vec3 sphereCenterRelBox = spherePos - boxPos;
	// Point on surface of box that is closest to the center of the sphere
	glm::vec3 boxPoint;
	// Check sphere center against box along the X axis alone. 
	// If the sphere is off past the left edge of the box, 
	// then the left edge is closest to the sphere. 
	// Similar if it's past the right edge. If it's between 
	// the left and right edges, then the sphere's own X 
	// is closest, because that makes the X distance 0.
	if (sphereCenterRelBox.x < -boxDimesions.x / 2)
		boxPoint.x = -boxDimesions.x / 2;
	else if (sphereCenterRelBox.x > boxDimesions.x / 2)
		boxPoint.x = boxDimesions.x / 2;
	else
		boxPoint.x = sphereCenterRelBox.x;
	// ...same for Y axis
	if (sphereCenterRelBox.y < -boxDimesions.y / 2.0)
		boxPoint.y = -boxDimesions.y / 2.0;
	else if (sphereCenterRelBox.y > boxDimesions.y / 2.0)
		boxPoint.y = boxDimesions.y / 2.0;
	else
		boxPoint.y = sphereCenterRelBox.y;

	// ... same for Z axis
	if (sphereCenterRelBox.z < -boxDimesions.z / 2.0)
		boxPoint.z = -boxDimesions.z / 2.0;
	else if (sphereCenterRelBox.z > boxDimesions.z / 2.0)
		boxPoint.z = boxDimesions.z / 2.0;
	else
		boxPoint.z = sphereCenterRelBox.z;

	// Now we have the closest point on the box, so get the distance from 
	// that to the sphere center, and see if it's less than the radius

	glm::vec3 dist = sphereCenterRelBox - boxPoint;

	if (dist.x*dist.x + dist.y*dist.y + dist.z*dist.z < sphereRad*sphereRad)
	{
		return true;
		
	}
	else
	{
		return false;
	}

}