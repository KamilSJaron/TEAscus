// *********************************************************************
//
// genome.cpp
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

#include "../include/Genome.h"

#define CHROMOSOMES 16
#define CHROM_LENGTH 500
#define RECOMBINATION 5.4

//int Genome::N = 0;
double Genome::u = 0;
double Genome::vt = 0;
double Genome::sa = 0;
double Genome::sb = 0;
int Genome::initialTE = 0;

std::random_device Genome::rd;
std::mt19937 Genome::mt(Genome::rd());

const int Genome::numberOfChromosomes = CHROMOSOMES;
const int Genome::chromLength = CHROM_LENGTH;
/// maybe I should turn chromRec to constant as well
double Genome::chromRecRate = RECOMBINATION;
//double Genome::rGenome = 0.01;

bool Genome::parametersSet = false;

// Random Genome::rand;

void Genome::SetParameters() {
	std::ifstream fin("input.txt");
	if (! fin.is_open())
		{std::cout << "Error opening file"; exit (1); }

	char tempChar[100];
	while(!fin.getline(tempChar, 100).eof()) {
		// fin.getline(tempChar,100); /// its read somewhere else
	//	N=strtol(tempChar,0,10);
		fin.getline(tempChar,100);
		u=strtod(tempChar,0);
		fin.getline(tempChar,100);
		vt=strtod(tempChar,0);
		fin.getline(tempChar,100);
		sa=strtod(tempChar,0);
		fin.getline(tempChar,100);
		sb=strtod(tempChar,0);
		fin.getline(tempChar,100);
		initialTE=strtol(tempChar,0,10);
	}
	fin.close();
	parametersSet = true;
}

Genome::Genome() {
	chromoVector.resize(numberOfChromosomes);
	if (!parametersSet)
		SetParameters();

	random = Random::Random();
	for (int i=1; i <= numberOfChromosomes; i++) {
		chromoVector.at(i-1).SetChromNumber(i);
	}
}

Genome::Genome(const Genome & rhs) {
	chromoVector.resize(numberOfChromosomes);

	if (!parametersSet)
		SetParameters();

	Locus * current;

	for (int i=1; i <= numberOfChromosomes; i++){
		chromoVector.at(i-1).SetChromNumber(i);
		current = rhs.GetChromosome(i).GetHeadLocus();
		while (current != 0) {
			chromoVector.at(i-1).Insert(current->GetPosition());
			current = current->GetNext();
		}
	}
}

unsigned int Genome::GetGenomeTECount() const {
	unsigned int genomeTEcount = 0;
	for (int i=1; i <= numberOfChromosomes; i++) {
		genomeTEcount += GetChromosome(i).GetChromTECount();
	}
	return genomeTEcount;
}

// input is chromosome number and copy number
const Chromosome & Genome::GetChromosome(int num) const {
		return chromoVector.at(num - 1);
}

Chromosome & Genome::GetChromosome(int num) {
		return chromoVector.at(num - 1);
}

double Genome::GetGenomeFitness() const
{
	unsigned int genomeTEcount = GetGenomeTECount();

	//return (1 - 0.001*pow(genomeTEcount, 1.5));

	// assume synergistic epistatic selection
	return exp ( -(sa * genomeTEcount) - (0.5 * sb * pow(genomeTEcount,2) ) );
}

// overloaded function when mean TE count is constant
double Genome::GetGenomeFitness(int meanCount) const {
	//return (1 - 0.001*pow(meanCount, 1.5));

	// assume synergistic epistatic selection
	return exp ( -(sa * meanCount) - (0.5 * sb * pow(meanCount,2) ) );
}

void Genome::SetChromosome(Chromosome & c) {
	int num = c.GetChromNumber();

	chromoVector.at(num - 1) = c;
}

void Genome::Transpose() {
	// std::cerr << "Transposing" << std::endl;
	int rolled_chromosome = 0, rolled_position_on_ch = 0;
	unsigned int transposeCount = 0;
	unsigned int totalLength = chromLength * numberOfChromosomes;
	std::poisson_distribution<int> rpois;

	int TEs = GetGenomeTECount();
	// std::cerr << "Proportion of gneome covered by TEs : " << TEs / (double)totalLength << std::endl;

	Locus * current;
	bool roll_again = true;
	// std::cerr << "setup" << std::endl;

	for (int ch=1; ch <= numberOfChromosomes; ch++){
		// std::cerr << "ch " << ch << std::endl;
		current = GetChromosome(ch).GetHeadLocus();
		while (current != 0) {
			// roll number of insertions
			// std::cerr << "rolling ";
			rpois = std::poisson_distribution<int>(u);
			transposeCount = rpois(mt);

			TEs += transposeCount;
			if ((TEs / (double)totalLength) > 0.8){
				std::cerr << "ERROR : an individual with more 80% of genome covered by TEs in simualtion." << std::endl;
				std::cerr << "Perhaps it would be good to modify parameters to meaningful values." << std::endl;
				exit (EXIT_FAILURE);
			}

			for (int t = 0; t < transposeCount; t++) {
				// std::cerr << "transposing locus" << std::endl;
				// roll where
				roll_again = true;
				do {
					random.ChromosomeAndPosition(& rolled_chromosome, & rolled_position_on_ch);
					// std::cerr << "stuff rolled, testing" << rolled_chromosome << " at position " << rolled_position_on_ch << std::endl;
					roll_again = !GetChromosome(rolled_chromosome).TestEmpty(rolled_position_on_ch);
					// std::cerr << "CH: " << rolled_chromosome << " TEs: " << GetChromosome(rolled_chromosome).GetChromTECount() << std::endl;
				} while (roll_again);

				chromoVector.at(rolled_chromosome - 1).Insert(rolled_position_on_ch);
			}
			// std::cerr << "GetNext ";
			current = current->GetNext();
			// std::cerr << "worked" << std::endl;
		}
	}
}

void Genome::ElementLoss() {
	if (vt == 0){
		return;
	}

	unsigned int lossCount=0, chromTEcount=0, nthTE=0;
	std::poisson_distribution<int> rpois;
	std::uniform_int_distribution<int> runif;

	for (int i=1; i <= numberOfChromosomes; i++) {
		chromTEcount = chromoVector.at(i-1).GetChromTECount();
		rpois = std::poisson_distribution<int>(vt*chromTEcount);
		lossCount = rpois(mt);
		// lossCount = (int)rand.Poisson(vt*chromTEcount);

		/// if by any chance poisson distribution generates number greater than number of TEs
		if (lossCount > chromTEcount) {
			/// just lose all of them
			lossCount = chromTEcount;
		}

		for (int k=0; k < lossCount; k++) {
			runif = std::uniform_int_distribution<int>(1, chromTEcount);
			nthTE = runif(mt);
			// nthTE = (int)(rand.Uniform()*chromTEcount + 1);
			chromoVector.at(i-1).Delete(nthTE);
			chromTEcount--;
		}
	}
}

void Genome::ListGenomeSites() const
{
	for (int i=0; i < numberOfChromosomes; i++){
		chromoVector.at(i).ListChromSites();
	}
	std::cout << std::endl;
}

int Genome::GetEmpiricalNumberOfChromosomes() {
	return(chromoVector.size());
}
