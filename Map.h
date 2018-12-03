//Map.h
#ifndef MAP_H
#define MAP_H
#include "PassportNew.h"

class cMap
{
private:
	sPassport m_passport;
public:
	cMap();
	int readPassport(char*);			// Read passport from SXF and save in sPassport
	int writePassportLog(char*);		// Write 
};

#endif