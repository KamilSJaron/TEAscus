// *********************************************************************
//
// Random.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#include <math.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "../include/Random.h"
#include "../include/Genome.h"

Random::Random() : mt((std::random_device())()) {}
Random::Random(int seed) : mt(seed) {}

int Random::NumberOfChiasmas(){
	std::poisson_distribution<int> rpois(Genome::chromRecRate);
	return(rpois(Random::mt));
}

int Random::GapPositionOnChromosome(){
	std::uniform_int_distribution<int> rgap(0, Genome::chromLength);
	return(rgap(Random::mt));
}

void Random::ChromosomeAndPosition(int * ch, int * p){
	/// in range from 1 to numberOfChromosomes
	std::uniform_int_distribution<int> rch(1, Genome::numberOfChromosomes);
	*ch = rch(mt);
	/// in range from 1 to chromLength
	std::uniform_int_distribution<int> rpos(1, Genome::chromLength);
	*p = rpos(mt);
}

int Random::TossACoin(){
	std::uniform_int_distribution<int> toss(0,1);
	return(toss(mt));
}