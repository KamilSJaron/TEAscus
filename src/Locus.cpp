// *********************************************************************
//
// locus.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
// Mofified by: Kamil S. Jaron, University of Lausanne
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#include <iostream>

#include "../include/Locus.h"

//Locus::Locus():
//	transElem(0),
//	next(0)
//	{}

Locus::Locus(int loc):
	location(loc),
	next(0) { }

Locus::Locus(int loc, Locus * n):
	location(loc),
	next(n)
	{}

Locus::Locus(const Locus & rhs) {
	location = rhs.GetPosition();
	next = rhs.GetNext();
}

Locus::~Locus() {
	delete next;
	next = 0;
	//std::cout << "Locus destructor w/ te: " << GetTransposon().GetLocation() << std::endl;
}

int Locus::GetPosition() const {
	return location;
}

Locus * Locus::GetNext() const {
	return next;
}

void Locus::SetNext(Locus * loc) {
	next = loc;
}