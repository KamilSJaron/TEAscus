// *********************************************************************
//
// TestPopulation.h
//
// Created by: Kamil S Jaron, University of Lausanne
//
// First started: Februrary, 2018
// Last edited:
//
// *********************************************************************


// file: TestPopulation.h
#ifndef POPULATIONTEST_H
#define POPULATIONTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../include/Population.h"

using namespace std;

class TestPopulation : public CPPUNIT_NS :: TestFixture {
	CPPUNIT_TEST_SUITE (TestPopulation);
	CPPUNIT_TEST (testSelectVitalIndividual);
	CPPUNIT_TEST (testGenerateOffspring);
	CPPUNIT_TEST (testAsexualReproduction);
	CPPUNIT_TEST (testSexualReproduction);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void testSelectVitalIndividual (void);
		void testGenerateOffspring (void);
		void testAsexualReproduction (void);
		void testSexualReproduction (void);

	private:
		Population *pop;
};

#endif
