#include "Hemicube.h"
#include <vector>
using namespace std;

vector<float> formfactor(vector<Plane> SPL, Plane patch);
bool inPatch(Plane patch, Position3 point);