#include "stdafx.h"
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include "Hemicube.h"

Hemicube::Hemicube(){
	//need to put this cube into a list.
}
void Hemicube::crossProduct(Position3 a, Position3 b, Position3 &c){
	//c always needs to be up. Hence, b X a.
	c.pos[0] = b.pos[1] * a.pos[2] - b.pos[2] * a.pos[1];
	c.pos[1] = -(b.pos[0] * a.pos[2] - b.pos[2] * a.pos[0]);
	c.pos[2] = b.pos[0] * a.pos[1] - b.pos[1] * a.pos[0];
}
void Hemicube::normalize(Position3 &a){
	//c always needs to be up. Hence, b X a.
	float k = 1 / sqrtf(a.pos[0] * a.pos[0] +
		a.pos[1] * a.pos[1] + a.pos[2] * a.pos[2]);
	for (int i = 0; i < 3; i++){
		a.pos[i] = a.pos[i] * k;
	}
		
}
void Hemicube::setHemicube(Plane plane, int iteration){
	iterations = iteration;
	Plane base = plane;
	for (int i = 0; i < 4; i++){
		position[i] = base.position[i];
	}
	//need nomral to cacalulate front view plane.
	Position3 a = position[1] - position[0];
	Position3 b = position[2] - position[0];
	crossProduct(a, b, normal);
	normalize(normal);
	//construct front view vertexes
	//find the center point using interpolation
	Position3 temp1,temp2;
	temp1.pos[0] = (position[1].pos[0] + position[0].pos[0]) / 2;
	temp1.pos[1] = (position[1].pos[1] + position[0].pos[1]) / 2;
	temp1.pos[2] = (position[1].pos[2] + position[0].pos[2]) / 2;
	temp2.pos[0] = (position[3].pos[0] + position[2].pos[0]) / 2;
	temp2.pos[1] = (position[3].pos[1] + position[2].pos[1]) / 2;
	temp2.pos[2] = (position[3].pos[2] + position[2].pos[2]) / 2;
	center.pos[0] = (temp1.pos[0] + temp2.pos[0]) / 2;
	center.pos[1] = (temp1.pos[0] + temp2.pos[0]) / 2;
	center.pos[2] = (temp1.pos[0] + temp2.pos[0]) / 2;
	//find center in front plane
	centerFront = center + normal;
	//find the front plane vertexes
	a = position[0] - position[3];
	b = position[0] - position[1];
	normalize(a);
	normalize(b);
	frontPosition[0] = centerFront + a + b;
	frontPosition[1] = centerFront + a - b;
	frontPosition[2] = centerFront - a - b;
	frontPosition[3] = centerFront - a + b;
	//extend the plane by 1 all around (top,bottom, left, right)
	extendedPosition[0] = frontPosition[0] + a + b;
	extendedPosition[1] = frontPosition[1] + a - b;
	extendedPosition[2] = frontPosition[2] - a - b;
	extendedPosition[3] = frontPosition[3] - a + b;
	extendedPlane.setPlane(extendedPosition[0], extendedPosition[1], extendedPosition[2], extendedPosition[3]);
	frontPlane.setPlane(frontPosition[0], frontPosition[1], frontPosition[2], frontPosition[3]);

	//divide plane into many patches and put them into a list.
	frontPatches = subdivide(frontPlane, iterations);
	//need width and height to calculate patches in other sides
	width = frontPatches[0].position[1] - frontPatches[0].position[0];
	height = frontPatches[0].position[2] - frontPatches[0].position[1];
	std::vector<Plane> tempLeftPatches1, tempRightPatches1, tempTopPatches1, tempBottomPatches1, 
		tempLeftPatches2, tempRightPatches2, tempTopPatches2, tempBottomPatches2;
	normalRight = b;
	normalLeft.pos[0] = -b.pos[0];
	normalLeft.pos[1] = -b.pos[1];
	normalLeft.pos[2] = -b.pos[2];
	normalBottom = a;
	normalTop.pos[0] = -a.pos[0];
	normalTop.pos[1] = -a.pos[1];
	normalTop.pos[2] = -a.pos[2];
	//divide the side plane by 2 equal square
	leftPlane1.setPlane(position[0], position[0] + normalTop, position[0] + normalTop - normal, position[0] - normal);
	leftPlane2.setPlane(position[0] + normalTop, position[3], position[3] - normal, position[0] + normalTop - normal);
	bottomPlane1.setPlane(position[3], position[3] + normalLeft, position[3] + normalLeft - normal, position[3] - normal);
	bottomPlane2.setPlane(position[3] + normalLeft, position[2], position[2] - normal, position[3] + normalLeft - normal);
	rightPlane1.setPlane(position[2], position[2] + normalBottom, position[2] + normalBottom - normal, position[2] - normal);
	rightPlane2.setPlane(position[2] + normalBottom, position[1], position[1] - normal, position[2] + normalBottom - normal);
	topPlane1.setPlane(position[1], position[1] + normalRight, position[1] + normalRight - normal, position[1] - normal);
	topPlane2.setPlane(position[1] + normalRight, position[0], position[0] - normal, position[1] + normalRight - normal);
	leftPatches1 = subdivide(leftPlane1, iterations / 2);
	leftPatches2 = subdivide(leftPlane2, iterations / 2);
	rightPatches1 = subdivide(rightPlane1, iterations / 2);
	rightPatches2 = subdivide(rightPlane2, iterations / 2);
	topPatches1 = subdivide(topPlane1, iterations / 2);
	topPatches2 = subdivide(topPlane2, iterations / 2);
	bottomPatches1 = subdivide(bottomPlane1, iterations / 2);
	bottomPatches2 = subdivide(bottomPlane2, iterations / 2);
	//put all in one
	//patches.reserve(frontPatches.size() + leftPatches1.size() + leftPatches2.size() + 
	//	rightPatches1.size() + rightPatches2.size() + topPatches1.size() + topPatches2.size + bottomPatches1.size() + bottomPatches2.size());
	patches.insert(patches.end(),frontPatches.begin(),frontPatches.end());
	patches.insert(patches.end(), leftPatches1.begin(), leftPatches1.end());
	patches.insert(patches.end(), leftPatches2.begin(), leftPatches2.end());
	patches.insert(patches.end(), topPatches2.begin(), topPatches2.end());
	patches.insert(patches.end(), topPatches1.begin(), topPatches1.end());
	patches.insert(patches.end(), rightPatches1.begin(), rightPatches1.end());
	patches.insert(patches.end(), rightPatches2.begin(), rightPatches2.end());
	patches.insert(patches.end(), bottomPatches1.begin(), bottomPatches1.end());
	patches.insert(patches.end(), bottomPatches2.begin(), bottomPatches2.end());
	/*
	leftPatches.push_back(leftPatches1);
	leftPatches.push_back(leftPatches2);
	rightPatches.push_back(rightPatches1);
	rightPatches.push_back(rightPatches2);
	bottomPatches.push_back(bottomPatches1);
	bottomPatches.push_back(bottomPatches2);
	topPatches.push_back(topPatches1);
	topPatches.push_back(topPatches2);*/
}