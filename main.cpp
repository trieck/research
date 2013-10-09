#include "stdafx.h"
#include "Octree.h"
#include "HiresTimer.h"

std::mt19937 engine;

/////////////////////////////////////////////////////////////////////////////
// Random number between [-1, 1]
float random() 
{ 
	std::mt19937::result_type max = engine.max();
	std::mt19937::result_type r = engine();
	return -1.f + (2.f * r) * (1.f / max);
}

/////////////////////////////////////////////////////////////////////////////
// Random vector with components in the the range [-1, 1]
Vector randomVec()
{ 
	return Vector(random(), random(), random()); 
}

/////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) 
{
	if (argc == 1) {
		cerr << "usage: octree points" << endl;
		exit(1);
	}

	HiresTimer timer;

	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	engine.seed(li.LowPart);

	// Create an octree centered at origin 
	// with physical dimensions 2x2x2
	Octree tree(Vector(0, 0, 0), Vector(1, 1, 1));

	// insert random points into the tree
	int npoints = atoi(argv[1]);
	for (int i = 0; i < npoints; ++i) {
		Vector vec = randomVec();
		tree.insert(vec);
	}

	// create a query region
	Region region(Vector(-.05f,-.05f,-.05f), Vector(.05f,.05f,.05f));

	DatumVec results;
	tree.query(region, results);

	cout << "   found " << results.size() << " points." << endl;
	cout << "   elapsed time " << timer << endl;

	return 0;
}
