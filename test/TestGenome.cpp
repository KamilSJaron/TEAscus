// simulator Transposon
// testing class Transposon

#include "../include/TestGenome.h"
#include "../include/Transposon.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestGenome);

using namespace CppUnit;
using namespace std;

void TestGenome::setUp(void) {
	Genome::SetParameters();
	Genome::u_mitosis = 0.1;
	ind1 = new Genome();
	Genome nice_folk;
	ind2 = new Genome( nice_folk );
	ind3 = new Genome( nice_folk );
}

void TestGenome::tearDown(void) {
	delete ind1;
}

void TestGenome::testConstructor(void) {
	int static_ch = Genome::numberOfChromosomes;
	int size_of_ind1_ch_vector = ind1->GetEmpiricalNumberOfChromosomes();
	// int size_of_ind2_ch_vector = ind2->GetEmpiricalNumberOfChromosomes();

	CPPUNIT_ASSERT_EQUAL(static_ch, size_of_ind1_ch_vector);
	// CPPUNIT_ASSERT_EQUAL(static_ch, size_of_ind2_ch_vector);
}

void TestGenome::testTransposonIteration(void){
	Transposon * current;
	for (int i=1; i <= Genome::numberOfChromosomes; i++) {
		current = ind1->GetChromosome(i).GetHeadTransposon();

		while (current != 0) {
			current = current->GetNext();
		}
	} // for
}

void TestGenome::testMitoticTranspose(void){
	unsigned int init_TEs = 100;
	int rolled_chromosome = 0, rolled_position_on_ch = 0;
	Random random;

	/// place random 100 TEs in ind1
	for (int j=0; j < init_TEs; j++) {
		do {
			random.ChromosomeAndPosition(& rolled_chromosome, & rolled_position_on_ch);
		} while ( !ind1->GetChromosome(rolled_chromosome).TestEmpty(rolled_position_on_ch));
		ind1->GetChromosome(rolled_chromosome).Insert(rolled_position_on_ch);
	}

	/// given high u and 100 TEs, at least one got to jump
	ind1->MitoticTranspose();

	CPPUNIT_ASSERT(ind1->GetGenomeTECount() > init_TEs);
}

// void TestGenome::testRandomness(void){
	// This test is due to random nature hard to test automatically
	// Hence it's sort of manual - uncoment secion and run it and be/not be satisfied

	// std::vector<int> generated_chiasmas;
	// std::vector<int> generated_positions;
	// std::vector<int> tosses_by_coin;
    //
	// int generated_number = -1;
    //
	// for (int ch = 1; ch <= 1; ch++){ //Genome::numberOfChromosomes
	// 	for (int i = 0; i < 10000; i++){
	// 		generated_chiasmas.push_back(Genome::GenerateNumberOfChiasmas(ch));
	// 	}
	// }
    //
	// int zeros = 0;
	// int genome_lengths = 0;
    //
	// for (int i = 0; i < 10000; i++){
	// 	generated_number = Genome::GenerateGapPositionOnChromosome();
		// if (generated_number == 0){
		// 	zeros++;
		// }
		// if(generated_number == Genome::chromLength){
		// 	genome_lengths++;
		// }
		// generated_positions.push_back(generated_number);
	// }

	// std::cerr << "\n" << "Proportion of zeros : " << (double)zeros / 10000 << "\n";
	// std::cerr << "Proportion of max : " << (double)genome_lengths / 10000 << "\n";
	// std::cerr << "expected proportion : " << (double)1 / (Genome::chromLength + 1) << "\n";

	// int ch_zeros = 0, p_zeros = 0;
	// int ch_max = 0, p_max = 0;
	// int rolled_position_on_ch = 0, rolled_chromosome = 0;
	// int replicates = 100000;
	//
	// for (int i = 0; i < replicates; i++){
	// 	Genome::GenerateChromosomeAndPosition(& rolled_chromosome, & rolled_position_on_ch);
	// 	if (rolled_position_on_ch == 0){
	// 		p_zeros++;
	// 	}
	// 	if (rolled_chromosome == 0){
	// 		ch_zeros++;
	// 	}
	// 	if(rolled_position_on_ch > Genome::chromLength){
	// 		p_max++;
	// 	}
	// 	if (rolled_chromosome > Genome::numberOfChromosomes){
	// 		ch_max++;
	// 	}
	// }
	//
	// CPPUNIT_ASSERT_EQUAL(0, p_zeros);
	// CPPUNIT_ASSERT_EQUAL(0, ch_zeros);
	// CPPUNIT_ASSERT_EQUAL(0, p_max);
	// CPPUNIT_ASSERT_EQUAL(0, ch_max);

	// int generated_sum = 0;
	// for (int i = 0; i < 1000000; i++){
	// 	generated_sum += Genome::GenerateTossACoin();
	// }
    //
	// std::cerr << "\n" << "Proportion of true : " << (double)generated_sum / 1000000 << "\n";
// }