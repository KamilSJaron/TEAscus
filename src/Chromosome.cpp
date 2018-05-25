// *********************************************************************
//
// chromosome.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
// Mofified by: Kamil S. Jaron, University of Lausanne
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
		headTransposon = 0;
	}

Chromosome::Chromosome(int num, int copy):
	chromNumber(num),
	chromCopy(copy)
	{
	headTransposon = 0;
	}

Chromosome::~Chromosome()
{
	delete headTransposon;
	headTransposon = 0;
}

unsigned int Chromosome::GetChromTECount() const
{
	unsigned int count = 0;
	Transposon * current = headTransposon;
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

Transposon * Chromosome::GetHeadTransposon() const
{
	return headTransposon;
}

void Chromosome::SetHeadTransposon(Transposon * newHead)
{
	headTransposon = newHead;
}

void Chromosome::SetChromNumber(int num) {
	chromNumber = num;
}

bool Chromosome::TestEmpty(int site) const
{
	Transposon * current = headTransposon;

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
	Transposon * newTransposon = new Transposon (location);

	if (headTransposon == 0)
	{
		headTransposon = newTransposon;
		return;
	}

	// if the new inserted locus is upstream of head, then make it the new head
	if (headTransposon->GetPosition() > location)
	{
		newTransposon->SetNext(headTransposon);
		headTransposon = newTransposon;
		return;
	}


	Transposon * currentTransposon = headTransposon;
	Transposon * nextTransposon = 0;
	int nextLocation = 0;

	for(;;)
	{
		// if there is no next, put new inserted locus at the end
		if (currentTransposon->GetNext() == 0)
		{
			currentTransposon->SetNext(newTransposon);
			return;
		}

		nextTransposon = currentTransposon->GetNext();
		nextLocation = nextTransposon->GetPosition();
		// std::cerr << nextLocation << std::endl;

		if (nextLocation > location)
		{
			currentTransposon->SetNext(newTransposon);
			newTransposon->SetNext(nextTransposon);
			return;
		}
		currentTransposon = nextTransposon;
	}
}

void Chromosome::Delete(int pos) {
	if ((pos > GetChromTECount()) || pos < 1) {
		std::cerr << "Deletion error." << std::endl;
		return;
	}

	Transposon * current = headTransposon;

	if (pos == 1) { // delete headTransposon
		headTransposon = headTransposon->GetNext();
		current->SetNext(0);
		delete current;
		current = 0;
		return;
	}

	for (int i=2; i<pos; i++)
		current = current->GetNext();

	Transposon * next = current->GetNext();
	current->SetNext(next->GetNext());
	next->SetNext(0);
	delete next;
	next = 0;
	return;
}

void Chromosome::ListChromSites() const {
	Transposon * loc = headTransposon;

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