#include "stdafx.h"
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include "Position3.h"
class Hemicube{
private:
	Position3 width, height, normal,normalLeft, normalRight, normalTop, normalBottom, center, centerFront, extendedPosition[4];
	Plane frontPlane, extendedPlane, leftPlane1, rightPlane1, topPlane1, bottomPlane1, leftPlane2, rightPlane2, topPlane2, bottomPlane2;
	int iterations;
	Position3 position[4],frontPosition[4];
	
	
public:
	std::vector<Plane> frontPatches, leftPatches1, rightPatches1, topPatches1, bottomPatches1,
		leftPatches2, rightPatches2, topPatches2, bottomPatches2;
	Hemicube();
	void setHemicube(Plane,int);
	void crossProduct(Position3, Position3, Position3);
	void normalize(Position3);
	
};


