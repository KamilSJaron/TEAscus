// *********************************************************************
//
// genome.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
// Mofified by: Kamil S. Jaron, University of Lausanne
//
// First started: March 11, 2005
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
double Genome::u_mitosis = 0;
double Genome::u_meiosis = 0;
double Genome::v_base = 0;
double Genome::v_modified = 0;
double Genome::sa = 0;
double Genome::sb = 0;
int Genome::initialTE = 0;

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
	// remove line with number of individuals & frequency of the general modifier
	fin.getline(tempChar,100);
	fin.getline(tempChar,100);
	fin.getline(tempChar,100);
	u_mitosis=strtod(tempChar,0);
	fin.getline(tempChar,100);
	u_meiosis=strtod(tempChar,0);
	fin.getline(tempChar,100);
	v_base=strtod(tempChar,0);
	fin.getline(tempChar,100);
	v_modified=strtod(tempChar,0);
	fin.getline(tempChar,100);
	sa=strtod(tempChar,0);
	fin.getline(tempChar,100);
	sb=strtod(tempChar,0);
	fin.getline(tempChar,100);
	initialTE=strtol(tempChar,0,10);

	fin.close();
	parametersSet = true;
}

Genome::Genome() {
	chromoVector.resize(numberOfChromosomes);
	if (!parametersSet)
		SetParameters();

	random = Random();
	modifier = 0;
	for (int i=1; i <= numberOfChromosomes; i++) {
		chromoVector.at(i-1).SetChromNumber(i);
	}
}

Genome::Genome(const Genome & rhs) {
	chromoVector.resize(numberOfChromosomes);

	if (!parametersSet)
		SetParameters();

	modifier = rhs.modifier;
	Transposon * current;

	for (int i=1; i <= numberOfChromosomes; i++){
		chromoVector.at(i-1).SetChromNumber(i);
		current = rhs.GetChromosome(i).GetHeadTransposon();
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

void Genome::SetModifierOn(){
	modifier = 1;
}

void Genome::insertTE() {
	int rolled_chromosome = 0, rolled_position_on_ch = 0;
	bool roll_again = true;
	do {
		random.ChromosomeAndPosition(& rolled_chromosome, & rolled_position_on_ch);
		// std::cerr << "stuff rolled, testing" << rolled_chromosome << " at position " << rolled_position_on_ch << std::endl;
		roll_again = !GetChromosome(rolled_chromosome).TestEmpty(rolled_position_on_ch);
		// std::cerr << "CH: " << rolled_chromosome << " TEs: " << GetChromosome(rolled_chromosome).GetChromTECount() << std::endl;
	} while (roll_again);

	chromoVector.at(rolled_chromosome - 1).Insert(rolled_position_on_ch);
}

void Genome::MitoticTranspose() {
	// std::cerr << "Transposing" << std::endl;
	unsigned int totalLength = chromLength * numberOfChromosomes;

	int TEs = GetGenomeTECount();
	/// roll how many new transposons will be generated
	int transposeCount = random.Binomial(TEs, u_mitosis);
	TEs += transposeCount;
	// std::cerr << "TEs: " << TEs << std::endl;
	// std::cerr << "totalLength: " << (double)totalLength << std::endl;
	// std::cerr << "transposeCount: " << transposeCount << std::endl;

	// std::cerr << "TEs : " << TEs  << " totalLength : " << totalLength << std::endl;
	if ((TEs / (double)totalLength) > 0.8){
		std::cerr << "ERROR : an individual with more 80% of genome covered by TEs in simualtion." << std::endl;
		std::cerr << "Perhaps it would be good to modify parameters - decrease transposition rate or increase selection or something." << std::endl;
		exit (EXIT_FAILURE);
	}

	for (int t = 0; t < transposeCount; t++) {
		insertTE();
	}
}

void Genome::ElementLoss() {
	if (v_base == 0){
		return;
	}

	unsigned int lossCount=0, chromTEcount=0, nthTE=0;
	std::poisson_distribution<int> rpois;
	std::uniform_int_distribution<int> runif;

	for (int i=1; i <= numberOfChromosomes; i++) {
		chromTEcount = chromoVector.at(i-1).GetChromTECount();
		if ( modifier == 0 ){
			lossCount = random.Binomial(chromTEcount, v_base);
		} else {
			lossCount = random.Binomial(chromTEcount, v_modified);
		}

		/// if by any chance poisson distribution generates number greater than number of TEs
		if (lossCount > chromTEcount) {
			/// just lose all of them
			lossCount = chromTEcount;
		}

		for (int k=0; k < lossCount; k++) {
			nthTE = random.randomTE(chromTEcount);
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
