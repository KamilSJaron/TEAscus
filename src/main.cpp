// *********************************************************************
//
// main.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
// Mofified by: Kamil S. Jaron, University of Lausanne
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************


#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cstring>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

#include "time.h"

#include "../include/Population.h"
#include "../include/gitversion.h"
#include "../include/Genome.h"

using namespace std;

int main(int argc, char **argv){

	/* constants - input and output files */
	const char *detailed_out = "detailed.txt";
	const char *input_file = "input.txt";
	bool sex = false;
	string runnig_sex = "False";

	int replicates = 10;
	int generations = 990;
	int sex_report_period = 90;
	int burnin = 20;
	int index;
	int c;
	double prerepTEs = 0, postrepTEs = 0, modifier_freq = 0;

	opterr = 0;

	while ((c = getopt (argc, argv, "hvsmb:r:g:p:i:o:")) != -1)
	switch (c)
		{
		case 'b':
			burnin = atoi(optarg);
			break;
		case 'r':
			replicates = atoi(optarg);
			break;
		case 'g':
			generations = atoi(optarg);
			break;
		case 'p':
			sex_report_period = atoi(optarg);
			break;
		case 's':
			sex = true;
			runnig_sex = "True";
			break;
		case 'i':
			input_file = optarg;
			break;
		case 'o':
			detailed_out = optarg;
			break;
		case 'h':
			cout << "most of parameters are read form an input file input.txt" << endl;
			cout << "details are in README.md file" << endl;
			cout << "Usage: \n \t TEAscus [options]" << endl;
			cout << "Options: \n \t [-h] \t \t print help of TEAscus and die" << endl
				<< " \t [-v] \t \t print version of TEAscus and die" << endl
				<< " \t [-s] \t \t sex every [-p] generations (False)" << endl
				<< " \t [-g] \t INT \t number of generations (990)" << endl
				<< " \t [-b] \t INT \t number of burn-in generations before simumlation (20)" << endl
				<< " \t [-r] \t INT \t number of replicates (10)" << endl
				<< " \t [-p] \t INT \t period of saves and sex if -s option is set (90)" << endl
				<< " \t [-i] \t FILE \t name of input file (input.txt)" << endl
				<< " \t [-o] \t FILE \t name of output file (detailed.txt)" << endl;
			return 0;
		case 'v':
			cout << "TEAscus fork of Transposon ( doi : 10.1534%2Fgenetics.106.060434 )" << endl;
			cout << "\thttps://github.com/KamilSJaron/TEAscus" << endl;
			cout << "\tcommit: " << GITVERSION << endl;
			return 0;
	 	case '?':
			if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr,
					"Unknown option character `\\x%x'.\n",
					optopt);
			return 1;
		default:
			abort ();
	  }

	for (index = optind; index < argc; index++)
		printf ("Non-option argument %s\n", argv[index]);

	cerr << "Running TEAscus" << endl;
	/* start of dev vesion */
	cerr << "Commit: " << GITVERSION << endl;
	/* end */
	cerr << "Input : " << input_file << endl;
	cerr << "Output : " << detailed_out << endl;
	cerr << "Number of generations : " << generations << endl;
	cerr << "Number of burn-in generations : " << burnin << endl;
	cerr << "Number of replicates : " << replicates << endl;
	cerr << "Sex every " << sex_report_period << " generations : " << runnig_sex << endl;

	// For testing input
	// return 0;

	for (int run=1; run <= replicates; run++) {
		std::cerr << "Running simualtion: " << run << endl;
		std::ifstream fin(input_file);
			if (! fin.is_open())
				{ cerr << "Error opening file : " << input_file << endl; exit (1); }

			// Initialize population size & whether to generate new population or load from file
			int N;
			char tempChar[100];
			fin.getline(tempChar,100);
			N=strtol(tempChar,0,10);
			fin.getline(tempChar,100);
			modifier_freq=strtod(tempChar,0);
		fin.close();

		Population * pop = new Population(N, modifier_freq);
		// cerr << "Population created" << endl;
		Population * new_population;

		int size = pop->GetPopSize();
		// cerr << "Population size : " << size << endl;
		pop->Initialize();
		// cerr << "Population initiated" << endl;
		pop->SaveParameters(detailed_out);

		if (run==1) {
			pop->PrintParameters();
		}

		/// Burn in serves to generate some variability
		for (int gen = 1; gen <= burnin; gen++){
			// cerr << "Runnin burnin" << endl;
			pop->MitoticTransposition();
			pop->Exision();
		}

		pop->SummaryStatistics(detailed_out, 0);

		for (int gen = 1; gen <= generations; gen++) {

			if (pop->GetPopulationTECount() == 0 or ((double)pop->GetPopulationTECount()/(double)size) > 150.0) {
				// cerr << "No TEs at generation [" << gen << "]." << endl << endl;
				// cerr << "Population extinction at generation [" << gen << "]." << endl << endl;
				pop->SummaryStatistics(detailed_out, gen);
				break;
			}

			// cerr << "Reproducing " << endl;
			// REPRODUCTION
			int popSize = pop->GetPopSize();
			prerepTEs = (double)pop->GetPopulationTECount() / popSize;
			if ( gen % sex_report_period == 0 and sex ){
				new_population = pop->SexualReproduction();
			} else {
				new_population = pop->AsexualReproduction();
				/// mitotic transpostion practically happens in the reproduced individuals
				new_population->MitoticTransposition();
			}
			postrepTEs = (double)new_population->GetPopulationTECount() / popSize;

			delete pop;
			pop = new_population;

			// cerr << "Transposon loss " << endl;
			/// LOSS
			pop->Exision();

			if(gen % 10 == 0){
				std::cout << gen << "\t" << prerepTEs << "\t" << postrepTEs << "\t" << pop->GetModifierFrequency() << std::endl;
			}
			/// printing results after transposition
			if (gen % sex_report_period == 0) {
				pop->SummaryStatistics(detailed_out, gen);
			}
		}

		delete pop;
		std::cerr << "DONE!" << endl;
	}

	return 0;
}
