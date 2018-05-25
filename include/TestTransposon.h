// *********************************************************************
//
// TestTransposon.h
//
// Created by: Kamil S Jaron, University of Lausanne
//
// First started: Februrary, 2018
// Last edited:
//
// *********************************************************************


#ifndef TRANSPTEST_H
#define TRANSPTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../include/Transposon.h"

using namespace std;

class TestTransposon : public CPPUNIT_NS :: TestFixture {
	CPPUNIT_TEST_SUITE (TestTransposon);
	CPPUNIT_TEST (testGetPosition);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void testGetPosition (void);

	private:
		Transposon *transposon1, *transposon2, *transposon3;
};

#endif
