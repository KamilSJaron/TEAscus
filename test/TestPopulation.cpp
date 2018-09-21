// simulator Transposon
// testing class Population

#include "../include/TestPopulation.h"
#include "../include/Genome.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestPopulation);

using namespace CppUnit;
using namespace std;

void TestPopulation::setUp(void) {
	Genome::initialTE = 3;
	Genome::u_mitosis = 0.001;
	pop = new Population(20, 0.2);
	pop->Initialize();
}

void TestPopulation::tearDown(void) {
	delete pop;
}

void TestPopulation::testSelectVitalIndividual(void) {
	int selected_ind = 0;
	selected_ind = pop->SelectVitalIndividual();
	// prints a matrix 30x30 of the rolled numbers - just for quick visual inspection
	// std::cerr << "\n";
	// for(int i = 0; i < 30; i++){
	// 	for(int j = 0; j < 30; j++){
	// 		std::cerr << pop->SelectVitalIndividual() << " ";
	// 	}
	// 	std::cerr << "\n";
	// }
	CPPUNIT_ASSERT(selected_ind >= 0 && selected_ind <= 19);
}

void TestPopulation::testGenerateOffspring(void) {
	// std::cerr << std::endl << "u_meiosis: " << Genome::u_meiosis << std::endl;
	Genome::u_meiosis = 0.1;
	int popSize = pop->GetPopSize();
	int pre_sex_TEs = pop->GetPopulationTECount();
	Population * newPopulation = new Population(popSize, 0.3);

	Genome parent1(pop->GetIndividual(pop->SelectVitalIndividual()));
	Genome parent2(pop->GetIndividual(pop->SelectVitalIndividual()));

	// std::cerr << std::endl << "Pre-sex TEs:  " << pop->GetPopulationTECount() << std::endl;
	for(int i = 0; i < popSize; i++){
		pop->generateOffspring(i, newPopulation, parent1, parent2);
	}
	// std::cerr << "Post-sex TEs: " << newPopulation->GetPopulationTECount() << std::endl;;
	Genome::u_meiosis = 0.0005;

	CPPUNIT_ASSERT(newPopulation->GetPopulationTECount() > pre_sex_TEs);
}

void TestPopulation::testSexualReproduction(void) {
	Population * temp;
	for( int i = 0; i < 100; i++){
		temp = pop->SexualReproduction();
		delete pop;
		pop = temp;
	}
}

void TestPopulation::testAsexualReproduction(void) {
	Population * temp;
	for( int i = 0; i < 100; i++){
		temp = pop->AsexualReproduction();
		delete pop;
		pop = temp;
	}
}