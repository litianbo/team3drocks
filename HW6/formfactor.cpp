#include "formfactor.h"

vector<float> formfactor(vector<Plane> SPL, Plane patch)
{
	vector<float> formfactorlist;
	Position3 O = Position3();
	
	/* Calculate the center of this patch */
	for (int i = 0; i < 3; i++)
		O.pos[i] = (patch.position[0].pos[i] + patch.position[1].pos[i] + patch.position[2].pos[i] + patch.position[3].pos[i]) / 4;

	/* Construct the hemicube list */
	Hemicube HPL = Hemicube();
	HPL.setHemicube(patch, 2);

	return formfactorlist;
}