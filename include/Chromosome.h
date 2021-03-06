// *********************************************************************
//
// chromosome.h
//
// Created by: Elie Dolgin, University of Edinburgh
// Mofified by: Kamil S. Jaron, University of Lausanne
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#ifndef CHROMOSOME_H_EDOLGIN_TE
#define CHROMOSOME_H_EDOLGIN_TE

#include "../include/Transposon.h"


class Chromosome
{
  public:
	Chromosome();
	Chromosome(int, int);
	~Chromosome();

	unsigned int GetChromTECount() const;
	unsigned int GetChromNumber() const;
	unsigned int GetChromCopy() const;
	Transposon * GetHeadTransposon() const;

	void SetHeadTransposon(Transposon *);
	void SetChromNumber(int);

	bool TestEmpty(int) const;

	void Insert(int);
	void Delete(int);

	void ListChromSites() const;


  private:
	Transposon * headTransposon;

	unsigned int chromNumber;	// chromosome number
	unsigned int chromCopy;		// denotes ploidy
};



#endif