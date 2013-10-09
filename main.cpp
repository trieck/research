#include "stdafx.h"
#include "Octree.h"
#include "HiresTimer.h"

/////////////////////////////////////////////////////////////////////////////
class OCTreeTest : public ICallable<Datum>
{
	// Construction / Destruction
public:
	OCTreeTest();
	~OCTreeTest() {}

	// Interface
	void test(int count);
	void Call(const Datum*, PVOID pExtra = NULL);

	// Implementation
private:
	Vector randomVec();
	float random();

	std::mt19937 engine;
};

/////////////////////////////////////////////////////////////////////////////
OCTreeTest::OCTreeTest()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	engine.seed(li.LowPart);
}

/////////////////////////////////////////////////////////////////////////////
// Random number between [-1, 1]
float OCTreeTest::random() 
{ 
	std::mt19937::result_type max = engine.max();
	std::mt19937::result_type r = engine();
	return -1.f + (2.f * r) * (1.f / max);
}

/////////////////////////////////////////////////////////////////////////////
// Random vector with components in the the range [-1, 1]
Vector OCTreeTest::randomVec()
{ 
	return Vector(random(), random(), random()); 
}

/////////////////////////////////////////////////////////////////////////////
void OCTreeTest::Call(const Datum* pData, PVOID pExtra)
{
	static int counter;

	const Vector& pt = pData->getPosition();

	fprintf(stdout, "#%d, x = %.6f, y = %.6f, z = %.6f\n", 
		++counter, pt.x, pt.y, pt.z);
}

/////////////////////////////////////////////////////////////////////////////
void OCTreeTest::test(int npoints)
{
	HiresTimer timer;

	// create an octree centered at origin 
	// with physical dimensions 2x2x2
	Octree tree(Vector(0, 0, 0), Vector(1, 1, 1));

	// insert random points into the tree
	string str;
	for (int i = 0; i < npoints; ++i) {
		Vector vec = randomVec();
		tree.insert(vec);		

		str.clear();
		tree.encode(vec, str);
	}

	// create a query region
	Region region(Vector(-.05f,-.05f,-.05f), Vector(.05f,.05f,.05f));

	// query the octree
	DatumVec results;
	tree.query(region, results);

	cout << "   found " << results.size() << " points." << endl;
	cout << "   elapsed time " << timer << endl;
}

/////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) 
{
	if (argc == 1) {
		cerr << "usage: octree points" << endl;
		exit(1);
	}

	OCTreeTest test;
	test.test(atoi(argv[1]));

	return 0;
}
