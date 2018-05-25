// *********************************************************************
//
// Transposon.h
//
// Created by: Elie Dolgin, University of Edinburgh
// Mofified by: Kamil S. Jaron, University of Lausanne
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#ifndef TRANSPOSON_H_EDOLGIN_TE
#define TRANSPOSON_H_EDOLGIN_TE

class Transposon
{
  public:
	//Transposon();
	Transposon(int loc);
	Transposon(int loc, Transposon * n);
	Transposon(const Transposon &);
	~Transposon();

	int GetPosition() const;
	Transposon * GetNext() const;
	void SetNext(Transposon * loc);

  private:
	int location;
	Transposon * next;

};


#endif