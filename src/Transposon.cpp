// *********************************************************************
//
// Transposon.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
// Mofified by: Kamil S. Jaron, University of Lausanne
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#include <iostream>

#include "../include/Transposon.h"

//Transposon::Transposon():
//	transElem(0),
//	next(0)
//	{}

Transposon::Transposon(int loc):
	location(loc),
	next(0) { }

Transposon::Transposon(int loc, Transposon * n):
	location(loc),
	next(n)
	{}

Transposon::Transposon(const Transposon & rhs) {
	location = rhs.GetPosition();
	next = rhs.GetNext();
}

Transposon::~Transposon() {
	delete next;
	next = 0;
	//std::cout << "Transposon destructor w/ te: " << GetTransposon().GetLocation() << std::endl;
}

int Transposon::GetPosition() const {
	return location;
}

Transposon * Transposon::GetNext() const {
	return next;
}

void Transposon::SetNext(Transposon * loc) {
	next = loc;
}