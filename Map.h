//Map.h
#ifndef MAP_H
#define MAP_H
#include "PassportNew.h"

class cMap
{
private:
	sPassport passport;
public:
	cMap();
	int readPassport(char*);
	int writePassportLog(char*);
};

#endif