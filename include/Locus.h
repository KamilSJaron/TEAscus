// *********************************************************************
//
// locus.h
//
// Created by: Elie Dolgin, University of Edinburgh
// Mofified by: Kamil S. Jaron, University of Lausanne
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#ifndef LOCUS_H_EDOLGIN_TE
#define LOCUS_H_EDOLGIN_TE

class Locus
{
  public:
	//Locus();
	Locus(int loc);
	Locus(int loc, Locus * n);
	Locus(const Locus &);
	~Locus();

	int GetPosition() const;
	Locus * GetNext() const;
	void SetNext(Locus * loc);

  private:
	int location;
	Locus * next;

};


#endif