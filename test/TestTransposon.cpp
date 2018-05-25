// simulator Transposon
// testing class Transposon

#include "../include/TestTransposon.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestTransposon);

using namespace CppUnit;
using namespace std;

void TestTransposon::setUp(void) {
	transposon1 = new Transposon(7);
	transposon2 = new Transposon(4, transposon1);
	transposon3 = new Transposon(0);
}

void TestTransposon::tearDown(void) {
	delete transposon2; /// destructor of Transposon destroys whole chain of loci
	delete transposon3;
}

void TestTransposon::testGetPosition(void) {
	int pos = transposon1->GetPosition();
	CPPUNIT_ASSERT_EQUAL(7, pos);
	pos = transposon2->GetPosition();
	CPPUNIT_ASSERT_EQUAL(4, pos);
	pos = transposon3->GetPosition();
	CPPUNIT_ASSERT_EQUAL(0, pos);
}