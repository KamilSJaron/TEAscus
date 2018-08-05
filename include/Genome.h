// *********************************************************************
//
// genome.h
//
// Created by: Elie Dolgin, University of Edinburgh
// Mofified by: Kamil S. Jaron, University of Lausanne
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#ifndef GENOME_H_TEASCUS
#define GENOME_H_TEASCUS

#include "../include/Chromosome.h"
#include "../include/Random.h"

#include <vector>
#include <random>

class Genome {

public:
	Genome();
	Genome(const Genome &);

	static void SetParameters();

	unsigned int GetGenomeTECount() const;
	const Chromosome & GetChromosome(int) const;
	Chromosome & GetChromosome(int);
	double GetGenomeFitness() const;
	double GetGenomeFitness(int) const;

	void SetChromosome(Chromosome&);

	void insertTE();
	void MitoticTranspose();
	void ElementLoss();
	void Recombination();
	//Ascus MakeAscus();
	void ListGenomeSites() const;

	/// just for testing
	int GetEmpiricalNumberOfChromosomes();

public:

	// class variables
//	static int N;					// Population size
	static double sa;				// selection coefficient alpha under synergistic epistasis
	static double sb;				// selection coefficient beta under synergistic epistasis
	static double u_mitosis;		// transposition rate for mitosis
	static double u_meiosis;		// transposition rate for meiosis
	static double vt;				// rate of element loss
	//static double rGenome;		// genome wide recombination rate between TE sites
	static int initialTE;
	const static int numberOfChromosomes;
	const static int chromLength;
	static double chromRecRate;
	static bool parametersSet;

private:

	Random random;

	std::vector<Chromosome> chromoVector;

};


#endif