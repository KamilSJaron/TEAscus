// *********************************************************************
//
// TestGenome.h
//
// Created by: Kamil S Jaron, University of Lausanne
//
// First started: Februrary, 2018
// Last edited:
//
// *********************************************************************

#ifndef GENOMETEST_H
#define GENOMETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../include/Genome.h"

using namespace std;

class TestGenome : public CPPUNIT_NS :: TestFixture {
	CPPUNIT_TEST_SUITE (TestGenome);
	CPPUNIT_TEST (testConstructor);
	CPPUNIT_TEST (testTransposonIteration);
	CPPUNIT_TEST (testMitoticTranspose);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void testConstructor (void);
		void testTransposonIteration (void);
		void testMitoticTranspose (void);

	private:
		Genome *ind1, *ind2, *ind3;
};

#endif
