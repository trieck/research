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

  std::unique_ptr<Octree> tree(new Octree(Vector(0, 0, 0), 1));

  int npoints = atoi(argv[1]);
  for (int i = 0; i < npoints; ++i) {
    Vector vec = randomVec();
    tree->insert(vec);
  }

  cout << "   elapsed time " << timer << endl;

  return 0;
}
