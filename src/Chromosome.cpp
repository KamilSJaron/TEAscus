// *********************************************************************
//
// chromosome.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#include <iostream>

#include "../include/Chromosome.h"

Chromosome::Chromosome():
	chromNumber(0),
	chromCopy(0)
	{
		headLocus = 0;
	}

Chromosome::Chromosome(int num, int copy):
	chromNumber(num),
	chromCopy(copy)
	{
	headLocus = 0;
	}

Chromosome::~Chromosome()
{
	delete headLocus;
	headLocus = 0;
}

unsigned int Chromosome::GetChromTECount() const
{
	unsigned int count = 0;
	Locus * current = headLocus;
	while (current != 0)
	{
		count++;
		current = current->GetNext();
	}
	return count;
}

unsigned int Chromosome::GetChromNumber() const
{
	return chromNumber;
}

unsigned int Chromosome::GetChromCopy() const
{
	return chromCopy;
}

Locus * Chromosome::GetHeadLocus() const
{
	return headLocus;
}

void Chromosome::SetHeadLocus(Locus * newHead)
{
	headLocus = newHead;
}

void Chromosome::SetChromNumber(int num) {
	chromNumber = num;
}

bool Chromosome::TestEmpty(int site) const
{
	Locus * current = headLocus;

	while ((current != 0) && (current->GetPosition() < site))
		current = current->GetNext();

	if (current == 0)
		return true;
	if (current->GetPosition() == site)
		return false;
	else
		return true;
}

void Chromosome::Insert (int location)
{
	Locus * newLocus = new Locus (location);

	if (headLocus == 0)
	{
		headLocus = newLocus;
		return;
	}

	// if the new inserted locus is upstream of head, then make it the new head
	if (headLocus->GetPosition() > location)
	{
		newLocus->SetNext(headLocus);
		headLocus = newLocus;
		return;
	}


	Locus * currentLocus = headLocus;
	Locus * nextLocus = 0;
	int nextLocation = 0;

	for(;;)
	{
		// if there is no next, put new inserted locus at the end
		if (currentLocus->GetNext() == 0)
		{
			currentLocus->SetNext(newLocus);
			return;
		}

		nextLocus = currentLocus->GetNext();
		nextLocation = nextLocus->GetPosition();
		// std::cerr << nextLocation << std::endl;

		if (nextLocation > location)
		{
			currentLocus->SetNext(newLocus);
			newLocus->SetNext(nextLocus);
			return;
		}
		currentLocus = nextLocus;
	}
}

void Chromosome::Delete(int pos) {
	if ((pos > GetChromTECount()) || pos < 1) {
		std::cerr << "Deletion error." << std::endl;
		return;
	}

	Locus * current = headLocus;

	if (pos == 1) { // delete headLocus
		headLocus = headLocus->GetNext();
		current->SetNext(0);
		delete current;
		current = 0;
		return;
	}

	for (int i=2; i<pos; i++)
		current = current->GetNext();

	Locus * next = current->GetNext();
	current->SetNext(next->GetNext());
	next->SetNext(0);
	delete next;
	next = 0;
	return;
}

void Chromosome::ListChromSites() const {
	Locus * loc = headLocus;

	std::cerr << "Chromosome " << chromNumber << " : ";
	if (loc != 0) {
		while (loc->GetNext() != 0) {
			std::cerr << loc->GetPosition() << ", ";
			loc = loc->GetNext();
		}
		std::cerr<<loc->GetPosition()<<".";
	}
	std::cerr << std::endl;
}