// simulator Transposon
// testing class Locus

#include "../include/TestLocus.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestLocus);

using namespace CppUnit;
using namespace std;

void TestLocus::setUp(void) {
	locus1 = new Locus(7);
	locus2 = new Locus(4, locus1);
	locus3 = new Locus(0);
}

void TestLocus::tearDown(void) {
	delete locus2; /// destructor of Locus destroys whole chain of loci
	delete locus3;
}

void TestLocus::testGetPosition(void) {
	int pos = locus1->GetPosition();
	CPPUNIT_ASSERT_EQUAL(7, pos);
	pos = locus2->GetPosition();
	CPPUNIT_ASSERT_EQUAL(4, pos);
	pos = locus3->GetPosition();
	CPPUNIT_ASSERT_EQUAL(0, pos);
}