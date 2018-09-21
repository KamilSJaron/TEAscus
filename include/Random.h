// *********************************************************************
//
// Random.h
//
// Created by: Kamil S Jaron, University of Lausanne
//
// First started: April 07, 2018
// Last edited:
//
// *********************************************************************

#ifndef RANDOM_H_TEASCUS
#define RANDOM_H_TEASCUS

#include "../include/Chromosome.h"

#include <vector>
#include <random>

class Random {

public:
	Random();
	Random(int seed);

	int NumberOfChiasmas();
	int GapPositionOnChromosome();
	void ChromosomeAndPosition(int * ch, int * p);
	int TossACoin();
	int randomTE(int te_number);
	int Poisson(double lambda);
	int Binomial(int count, double prob);

private:

	std::mt19937 mt;
};


#endif