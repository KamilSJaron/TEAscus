// *********************************************************************
//
// population.h
//
// Created by: Elie Dolgin, University of Edinburgh
// Mofified by: Kamil S. Jaron, University of Lausanne
//
// First started: March 17, 2005
// Last edited:
//
// *********************************************************************

#ifndef POPULATION_H_EDOLGIN_TE
#define POPULATION_H_EDOLGIN_TE

#include "../include/Genome.h"
#include <vector>


class Population {

public:
	Population(int);
	~Population();

	Genome & GetIndividual(int);
	const Genome & GetIndividual(int) const;
	int GetPopSize() const;
	unsigned int GetPopulationTECount() const;
	double GetPopulationMeanFitness() const;

	void Initialize();
	int SelectVitalIndividual();
	Genome MakeIndividual();
	void DeleteIndividual(int);
	Population * SexualReproduction();
	Population * AsexualReproduction();
	void MitoticTransposition();
	// void MeioticTransposition(Genome &);
	void Exision();
	/// sexual offpring - fusion of parents, recombination, creation of two complementary offsprings
	void generateOffspring(int ind, Population * newPopulation, Genome parent1, Genome parent2);

	void ListPopulationSites() const;
	void SaveParameters(const char *);
	void PrintParameters();
	void SummaryStatistics(const char *, int);
	void RecordPopulation(const char *, int);

	void SummaryStatistics(int);
	void SummaryStatistics(int, int);

private:
	std::vector<Genome> genoVector;
	int popSize;
	std::uniform_int_distribution<int> rind;
	std::uniform_real_distribution<double> runif;

	Random random;
	static std::random_device rd;
	static std::mt19937 mt;


	int getTransposonPosition(Transposon *) const;
};


#endif


