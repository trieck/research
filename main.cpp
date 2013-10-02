#include "Octree.h"
#include <memory>

int main(int argc, char** argv) 
{
	std::auto_ptr<Octree> tree(new Octree(Vector(0, 0, 0), 1));

	tree->insert(Vector(-.3, -.3, -.3));

	return 0;
}

