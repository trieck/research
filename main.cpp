#include "stdafx.h"
#include "Octree.h"
#include "HiresTimer.h"

using std::cout;
using std::cerr;
using std::endl;

// Random number between [-1,1]
float random() 
{ 
  return -1.f + (2.f*rand()) * (1.f / RAND_MAX); 
}

// Random vector with components in the the range [-1, 1]
Vector randomVec()
{ 
  return Vector(random(), random(), random()); 
}

int main(int argc, char** argv) 
{
  if (argc == 1) {
    cerr << "usage: octree points" << endl;
    exit(1);
  }

  HiresTimer timer;
  
  std::unique_ptr<Octree> tree(new Octree(Vector(0, 0, 0), 1));

  int npoints = atoi(argv[1]);
  for (int i = 0; i < npoints; ++i) {
    Vector vec = randomVec();
    tree->insert(vec);
  }

  cout << "   elapsed time " << timer << endl;

	return 0;
}

