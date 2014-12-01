#include "formfactor.h"

vector<float> formfactor(vector<Plane> SPL, Plane patch)
{
	vector<float> formfactorlist(SPL.size());
	Position3 O = Position3();

	/* Calculate the center of this patch */
	for (int i = 0; i < 3; i++)
		O.pos[i] = (patch.position[0].pos[i] + patch.position[1].pos[i] + patch.position[2].pos[i] + patch.position[3].pos[i]) / 4;

	/* Construct the hemicube list */
	Hemicube HPL = Hemicube();
	HPL.setHemicube(patch, 2);

	vector<Plane>::iterator itr = HPL.patches.begin();
	while (itr != HPL.patches.end())
	{
		/* Calculate the center of this hemicube patch */
		Plane hp = *itr;
		Position3 P = Position3();
		for (int i = 0; i < 3; i++)
			P.pos[i] = (hp.position[0].pos[i] + hp.position[1].pos[i] + hp.position[2].pos[i] + hp.position[3].pos[i]) / 4;

		for (int i = 0; i < SPL.size(); i++)
		{
			Position3 P0, P1, P2, P3;
			P1 = SPL[i].position[0];
			P2 = SPL[i].position[1];
			P3 = SPL[i].position[2];

			float u, v, w, t;
			u = (P2.pos[1] - P1.pos[1])*(P3.pos[2] - P1.pos[2]) - (P2.pos[2] - P1.pos[2])*(P3.pos[1] - P1.pos[1]);
			v = (P2.pos[2] - P1.pos[2])*(P3.pos[0] - P1.pos[0]) - (P2.pos[0] - P1.pos[0])*(P3.pos[2] - P1.pos[2]);
			w = (P2.pos[0] - P1.pos[0])*(P3.pos[1] - P1.pos[1]) - (P2.pos[1] - P1.pos[1])*(P3.pos[0] - P1.pos[0]);
			t = (P1.pos[0] * u + P1.pos[1] * v + P1.pos[2] * w - O.pos[0] - O.pos[1] - O.pos[2]) /
				(O.pos[0] + O.pos[1] + O.pos[2] - P.pos[0] - P.pos[1] - P.pos[2]);

			for (int j = 0; j < 3; j++)
				P0.pos[j] = O.pos[j] + t*(O.pos[j] - P.pos[j]);

			if (inPatch(SPL[i], P0)) formfactorlist[i] += 1;
		}
	}

	return formfactorlist;
}

bool inPatch(Plane patch, Position3 point)
{

}