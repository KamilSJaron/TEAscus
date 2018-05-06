// *********************************************************************
//
// Random.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
// Mofified by: Kamil S. Jaron, University of Lausanne
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
	return(Random::Poisson(Genome::chromRecRate));
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

int Random::randomTE(int te_number){
	std::uniform_int_distribution<int> runif(1, te_number);
	return(runif(mt));
}

int Random::Poisson(double lambda){
	std::poisson_distribution<int> rpois(lambda);
	return(rpois(Random::mt));
}