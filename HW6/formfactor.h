#include "Hemicube.h"
#include <vector>
using namespace std;

vector<float> formfactor(vector<Plane> SPL, Plane patch);
bool inPatch(Plane patch, Position3 point);
float d(Position3 u, Position3 v, int i, int j);
float LEE(Position3 tail, Position3 head, Position3 point, int i, int j);